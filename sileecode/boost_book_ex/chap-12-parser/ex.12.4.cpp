#include <boost/spirit/include/qi.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::spirit; 

template <typename Iterator, typename Skipper> 
struct json : qi::grammar<Iterator, Skipper> 
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

  qi::rule<Iterator, Skipper> object, member, str, value, number, array; 
}; 

int main() 
{ 
  std::string s = "{ \"Boris Schaeling\" : { \"Male\": true, " 
                  "\"Programming Languages\": [ \"C++\", \"Java\", " 
                  "\"C#\" ], \"Age\": 33 } }"; 
  json<std::string::iterator, ascii::space_type> json_parser; 
  bool success = qi::phrase_parse(s.begin(), s.end(), json_parser, 
    ascii::space); 
  std::cout << (success ? "OK" : "Error") << std::endl; 
} 