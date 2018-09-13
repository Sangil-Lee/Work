#include <boost/spirit/include/qi.hpp> 
#include <boost/fusion/include/adapt_struct.hpp> 
#include <boost/variant.hpp> 
#include <boost/foreach.hpp> 
#include <string> 
#include <vector> 
#include <iostream> 

using namespace boost::spirit; 

struct json_object; 
struct json_array; 

typedef boost::variant<std::string, double, 
  boost::recursive_wrapper<json_object>, 
  boost::recursive_wrapper<json_array> > json_value; 

struct json_array 
{ 
  std::vector<json_value> array; 
}; 

BOOST_FUSION_ADAPT_STRUCT( 
  json_array, 
  (std::vector<json_value>, array) 
) 

struct json_member 
{ 
  std::string key; 
  json_value value; 
}; 

BOOST_FUSION_ADAPT_STRUCT( 
  json_member, 
  (std::string, key) 
  (json_value, value) 
) 

struct json_object 
{ 
  std::vector<json_member> members; 
}; 

BOOST_FUSION_ADAPT_STRUCT( 
  json_object, 
  (std::vector<json_member>, members) 
) 

template <typename Iterator, typename Skipper> 
struct json : qi::grammar<Iterator, json_object(), Skipper> 
{ 
  json() 
    : json::base_type(object) 
  { 
    object %= "{" >> member % "," >> "}"; 
    member %= str >> ":" >> value; 
    str %= qi::lexeme["\"" >> *~qi::lit('"') >> "\""]; 
    value %= str | number | object | array | qi::string("true") | 
      qi::string("false") | qi::string("null"); 
    number %= qi::double_; 
    array %= "[" >> value % "," >> "]"; 
  } 

  qi::rule<Iterator, json_object(), Skipper> object; 
  qi::rule<Iterator, json_member(), Skipper> member; 
  qi::rule<Iterator, std::string(), Skipper> str; 
  qi::rule<Iterator, json_value(), Skipper> value; 
  qi::rule<Iterator, double(), Skipper> number; 
  qi::rule<Iterator, json_array(), Skipper> array; 
}; 

struct output : 
  public boost::static_visitor<> 
{ 
  void operator()(const std::string &s) const 
  { 
    std::cout << s << std::endl; 
  } 

  void operator()(double d) const 
  { 
    std::cout << d << std::endl; 
  } 

  void operator()(const json_object &o) const 
  { 
    BOOST_FOREACH(json_member member, o.members) 
    { 
      std::cout << member.key << std::endl; 
      boost::apply_visitor(output(), member.value); 
    } 
  } 

  void operator()(const json_array &a) const 
  { 
    BOOST_FOREACH(json_value value, a.array) 
    { 
      boost::apply_visitor(output(), value); 
    } 
  } 
}; 

int main() 
{ 
  std::string s = "{ \"Boris Schaeling\" : { \"Male\": true, " 
                  "\"Programming Languages\": [ \"C++\", \"Java\", " 
                  "\"C#\" ], \"Age\": 33 } }"; 
  json<std::string::iterator, ascii::space_type> json_parser; 
  json_object json_data; 
  bool success = qi::phrase_parse(s.begin(), s.end(), json_parser, 
    ascii::space, json_data); 
  if (success) 
  { 
    BOOST_FOREACH(json_member member, json_data.members) 
    { 
      std::cout << member.key << std::endl; 
      boost::apply_visitor(output(), member.value); 
    } 
  } 
} 