#include <stdio.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void hello_from_c(void)
{
    printf("Hello from C\n");
}
