#include <node.h>
#include <v8.h>
#include <uv.h>

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

static std::unique_ptr<node::MultiIsolatePlatform> platform;
static std::unique_ptr<node::ArrayBufferAllocator> allocator;
static std::unique_ptr<node::CommonEnvironmentSetup> setup;

static v8::Isolate* isolate = nullptr;
static node::IsolateData* isolate_data = nullptr;
static node::Environment* env = nullptr;

static v8::Global<v8::Context> context;

static bool initialized = false;


EXPORT int js_init(void)
{
    if (initialized)
        return 0;

    std::vector<std::string> args = {
        "node"
    };

    std::vector<std::string> exec_args;
    std::vector<std::string> errors;

    auto result =
        node::InitializeOncePerProcess(
            args,
            {
                node::ProcessInitializationFlags::kNoInitializeV8,
                node::ProcessInitializationFlags::kNoInitializeNodeV8Platform
            }
        );

    if (result->exit_code() != 0)
        return -1;

    platform =
        node::MultiIsolatePlatform::Create(1);

    if (!platform)
        return -2;

    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    setup =
        node::CommonEnvironmentSetup::Create(
            platform.get(),
            &errors,
            args,
            exec_args
        );

    if (!setup)
        return -3;

    isolate = setup->isolate();

    if (!isolate)
        return -4;

    isolate_data = setup->isolate_data();

    if (!isolate_data)
        return -5;

    env = setup->env();

    if (!env)
        return -6;

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        v8::Context::Scope context_scope(
            setup->context()
        );

        context.Reset(
            isolate,
            setup->context()
        );
    }

    initialized = true;

    return 0;
}


EXPORT int js_load(const char* filename)
{
    if (!initialized || !isolate || !env)
        return -1;

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Context::Scope context_scope(
        setup->context()
    );

    std::string script =
        "const publicRequire = "
        "require('node:module').createRequire(process.cwd() + '/');"
        "globalThis.require = publicRequire;"
        "require('node:vm').runInThisContext("
        "require('node:fs').readFileSync(";

    // Escape the filename for a JS string.
    script += "'";
    
    for (const char* p = filename; *p; ++p) {
        if (*p == '\\')
            script += "\\\\";
        else if (*p == '\'')
            script += "\\'";
        else
            script += *p;
    }

    script += "', 'utf8'));";

    auto result =
        node::LoadEnvironment(
            env,
            script
        );

    if (result.IsEmpty())
        return -2;

    return 0;
}


EXPORT int js_call_hello(void)
{
    if (!initialized ||
        !isolate ||
        !env)
        return -1;

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> ctx =
        context.Get(isolate);

    v8::Context::Scope context_scope(ctx);

    v8::TryCatch try_catch(isolate);

    v8::Local<v8::String> source =
        v8::String::NewFromUtf8Literal(
            isolate,
            "hello_from_js();"
        );

    v8::Local<v8::Script> script;

    if (!v8::Script::Compile(
            ctx,
            source
        ).ToLocal(&script))
        return -2;

    v8::Local<v8::Value> result;

    if (!script->Run(ctx).ToLocal(&result)) {
        if (try_catch.HasCaught()) {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            fprintf(stderr, "JavaScript error: %s\n",
                    *error ? *error : "<unknown>");
        }
        return -3;
    }

    return 0;
}


EXPORT void js_destroy(void)
{
    if (!initialized)
        return;

    if (env) {
        node::FreeEnvironment(env);
        env = nullptr;
    }

    if (isolate_data) {
        node::FreeIsolateData(
            isolate_data
        );

        isolate_data = nullptr;
    }

    context.Reset();

    if (isolate) {
        isolate->Dispose();
        isolate = nullptr;
    }

    allocator.reset();

    if (platform) {
        v8::V8::Dispose();
        v8::V8::DisposePlatform();

        platform.reset();
    }

    node::TearDownOncePerProcess();

    initialized = false;
}
