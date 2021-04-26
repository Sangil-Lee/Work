#include <cstdlib>

#include "aaa_c_connector.h"
#include "aaa.h"

#ifdef __cplusplus
extern "C" {
#endif

// Inside this "extern C" block, I can implement functions in C++, which will externally 
//   appear as C functions (which means that the function IDs will be their names, unlike
//   the regular C++ behavior, which allows defining multiple functions with the same name
//   (overloading) and hence uses function signature hashing to enforce unique IDs),


static AAA *AAA_instance = NULL;

void lazyAAA() {
    if (AAA_instance == NULL) {
        AAA_instance = new AAA();
    }
}

void AAA_sayHi(const char *name) {
    lazyAAA();
    AAA_instance->sayHi(name);
}

#ifdef __cplusplus
}
#endif


//g++ -fpic -shared aaa_c_connector.cpp -L. -laaa -o libaaa_c_connector.so
