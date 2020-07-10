#ifndef OCRASYNEPICS_H
#define OCRASYNEPICS_H

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40300
#include <tr1/unordered_map>
#define hash_map std::tr1::unordered_map
#else
#include <ext/hash_map>
#define hash_map __gnu_cxx::hash_map
namespace __gnu_cxx {
    template <>
        struct hash<std::string> {
            size_t operator() (const std::string& x) const {
                return hash<const char*>()(x.c_str());
            }
        };
};
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include <paramVal.h>
#include <asynParamType.h>
#include <ParamListInvalidIndex.h>
#include <ParamValWrongType.h>
#include <ParamValNotDefined.h>
#include <asynPortDriver.h>

using namespace std;

extern "C" {

};

class OCRAsynDriver : public asynPortDriver
{
public:
    OCRAsynDriver();
    ~OCRAsynDriver();


protected:

};


#endif // OCRASYNEPICS_H
