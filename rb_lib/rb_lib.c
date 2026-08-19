#include <ruby.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static int ruby_initialized = 0;

EXPORT int polyglot_rb_init(void)
{
    if (ruby_initialized)
        return 0;

    int argc = 1;
    char *argv_data[] = {
        "python",
        NULL
    };
    char **argv = argv_data;

    ruby_sysinit(&argc, &argv);

    RUBY_INIT_STACK;

    if (ruby_setup() != 0)
        return -1;

    ruby_init_loadpath();

    ruby_initialized = 1;

    return 0;
}

EXPORT int polyglot_rb_load(const char *filename)
{
    if (!ruby_initialized)
        return -1;

    VALUE path = rb_str_new_cstr(filename);

    rb_load(path, 0);

    return 0;
}

EXPORT int polyglot_rb_call_hello(void)
{
    if (!ruby_initialized)
        return -1;

    ID method = rb_intern("hello_from_rb");

    rb_funcall(rb_cObject, method, 0);

    return 0;
}

EXPORT void polyglot_rb_destroy(void)
{
    if (!ruby_initialized)
        return;

    ruby_cleanup(0);

    ruby_initialized = 0;
}
