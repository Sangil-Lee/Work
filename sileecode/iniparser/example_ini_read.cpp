#include "iniparser.hpp"
#include <iostream>

int main(void) {
    INI::File ft;
    if(ft.Load("example1.ini")){
        std::string x       = ft.GetSection("EXAMPLE1")->GetValue("x","").AsString();
        int y               = ft.GetSection("EXAMPLE1")->GetValue("y",0).AsInt();
        std::vector<int> z  = ft.GetSection("EXAMPLE1")->GetValue("z").AsArray().ToVector<int>();
        bool t              = ft.GetSection("EXAMPLE1")->GetValue("t",false).AsBool();

        std::cout << "x is " << x << std::endl;
        std::cout << "y is " << y << std::endl;
        for(int idx = 0; idx < z.size(); idx++) std::cout << "z[" << idx << "] is " << z[idx] << std::endl;
        std::cout << "t is " << t << std::endl;
    }
}
