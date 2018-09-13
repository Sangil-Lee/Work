#include <boost/spirit/include/qi.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::spirit; 

template <typename Iterator> 
struct json : qi::grammar<Iterator> 
{ 
  json() 
    : json::base_type(object) 
  { 
    object = "{" >> member >> *("," >> member) >> "}"; 
    member = str >> ":" >> value; 
    str = "\"" >> *~qi::lit('"') >> "\""; 
    value = str | number | object | array | "true" | "false" | "null"; 
    number = qi::double_; 
    array = "[" >> value >> *("," >> value) >> "]"; 
  } 

  qi::rule<Iterator> object, member, str, value, number, array; 
}; 

int main() 
{ 
  std::string s = "{\"Boris Schaeling\":{\"Male\":true,\"Programming " 
                  "Languages\":[\"C++\",\"Java\",\"C#\"],\"Age\":33}}"; 
  json<std::string::iterator> json_parser; 
  bool success = qi::parse(s.begin(), s.end(), json_parser); 
  std::cout << (success ? "OK" : "Error") << std::endl; 
} 