#include <iostream>

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

EXPORT void hello_from_cpp() {
    std::cout << "hello from C++" << std::endl;
}
