#include "iniparser.hpp"
#include <iostream>

int main(void) {
    INI::File ft;
    ft.SetValue("EXAMPLE2:x", "abc");
    ft.SetArrayValue("EXAMPLE2:y", 0, 1);
    ft.SetArrayValue("EXAMPLE2:y", 1, 2);
    ft.SetArrayValue("EXAMPLE2:y", 2, 3);
    ft.SetValue("EXAMPLE2:z", "xyz");
    ft.SetValue("EXAMPLE2:a", true);
    ft.Save("example2.ini");
    return 0;
}
