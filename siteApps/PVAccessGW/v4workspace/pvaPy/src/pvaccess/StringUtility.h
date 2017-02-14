// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <string>
#include <sstream>

namespace StringUtility 
{

template<typename T>
std::string toString(const T& t);
std::string toString(const char* s);
std::string toString(char* s);
std::string toString(bool b);

std::string& leftTrim(std::string& s);
std::string& rightTrim(std::string& s); 
std::string& trim(std::string& s);
std::string trim(const std::string& s);

//
// Template implementation
//
template<typename T>
std::string toString(const T& t)
{
    std::ostringstream result;
    result << t;
    return result.str();
}

}

#endif 


