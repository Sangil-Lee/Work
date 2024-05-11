#include <functional>
#include <iostream>
#include <memory_resource>
#include <string>
#include <string_view>
using namespace std::literals;
 
int main()
{
    auto sv = "Stand back! I've got jimmies!";
    std::string s(sv);
    std::pmr::string pmrs(sv); // use default allocator
 
    std::cout << std::hash<std::string_view>{}(sv) << '\n';
    std::cout << std::hash<std::string>{}(s) << '\n';
    std::cout << std::hash<std::pmr::string>{}(pmrs) << '\n';
}
