#include <jni.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static JavaVM *jvm = NULL;
static JNIEnv *env = NULL;
static jclass java_class = NULL;
static jmethodID hello_method = NULL;

EXPORT int java_init(void)
{
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    options[0].optionString = "-Djava.class.path=java_lib";

    vm_args.version = JNI_VERSION_21;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_TRUE;

    jint result = JNI_CreateJavaVM(
        &jvm,
        (void **)&env,
        &vm_args
    );

    if (result != JNI_OK)
        return -1;

    return 0;
}

EXPORT int java_load(void)
{
    java_class = (*env)->FindClass(
        env,
        "java_lib"
    );

    if (java_class == NULL)
        return -1;

    java_class = (*env)->NewGlobalRef(
        env,
        java_class
    );

    hello_method = (*env)->GetStaticMethodID(
        env,
        java_class,
        "hello_from_java",
        "()V"
    );

    if (hello_method == NULL)
        return -2;

    return 0;
}

EXPORT int java_call_hello(void)
{
    if (env == NULL ||
        java_class == NULL ||
        hello_method == NULL)
        return -1;

    (*env)->CallStaticVoidMethod(
        env,
        java_class,
        hello_method
    );

    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return -2;
    }

    return 0;
}

EXPORT void java_destroy(void)
{
    if (jvm != NULL) {
        (*jvm)->DestroyJavaVM(jvm);
        jvm = NULL;
        env = NULL;
    }
}
