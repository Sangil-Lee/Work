#include <boost/spirit/include/qi.hpp> 
#include <string> 
#include <iostream> 

using namespace boost::spirit; 

void print(const std::string &s) 
{ 
  std::cout << s << std::endl; 
} 

template <typename Iterator, typename Skipper> 
struct json : qi::grammar<Iterator, Skipper> 
{ 
  json() 
    : json::base_type(object) 
  { 
    object = "{" >> member >> *("," >> member) >> "}"; 
    member = str[print] >> ":" >> value; 
    str = qi::lexeme["\"" >> *~qi::lit('"') >> "\""]; 
    value = str | number | object | array | "true" | "false" | "null"; 
    number = qi::double_; 
    array = "[" >> value >> *("," >> value) >> "]"; 
  } 

  qi::rule<Iterator, Skipper> object, member, value, number, array; 
  qi::rule<Iterator, std::string(), Skipper> str; 
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