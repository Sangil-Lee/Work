#include <boost/spirit/include/qi.hpp> 

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

int main() {} 