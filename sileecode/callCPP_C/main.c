#include "aaa_c_connector.h"

int main() {
    AAA_sayHi("David");
    AAA_sayHi("James");

    return 0;
}


//gcc main.c -L. -laaa_c_connector -o c_aaa
// or Some linux distributsions,
//gcc main.c -L. -laaa_c_connector -laaa -lstdc++ -o c_aaa
