#include <EXTERN.h>
#include <perl.h>
#include <stdlib.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static PerlInterpreter *my_perl = NULL;

EXPORT int pl_init(void)
{
    _putenv("LANG=C");
    _putenv("LC_ALL=C");

    int argc = 3;

    char *argv_data[] = {
        "perl",
        "-e",
        "0",
        NULL
    };

    char **argv = argv_data;
    char **env = NULL;

    PERL_SYS_INIT3(&argc, &argv, &env);

    my_perl = perl_alloc();

    if (my_perl == NULL)
        return -1;

    perl_construct(my_perl);

    if (perl_parse(my_perl, NULL, argc, argv, NULL) != 0)
        return -2;

    if (perl_run(my_perl) != 0)
        return -3;

    return 0;
}

EXPORT int pl_load(const char *filename)
{
    if (my_perl == NULL)
        return -1;

    PERL_SET_CONTEXT(my_perl);

    char code[4096];

    snprintf(
        code,
        sizeof(code),
        "do '%s';",
        filename
    );

    eval_pv(code, TRUE);

    if (SvTRUE(ERRSV))
        return -2;

    return 0;
}

EXPORT int pl_call_hello(void)
{
    if (my_perl == NULL)
        return -1;

    PERL_SET_CONTEXT(my_perl);

    eval_pv("hello_from_pl();", TRUE);

    if (SvTRUE(ERRSV))
        return -2;

    return 0;
}

EXPORT void pl_destroy(void)
{
    if (my_perl == NULL)
        return;

    PERL_SET_CONTEXT(my_perl);

    perl_destruct(my_perl);
    perl_free(my_perl);

    my_perl = NULL;

    PERL_SYS_TERM();
}
