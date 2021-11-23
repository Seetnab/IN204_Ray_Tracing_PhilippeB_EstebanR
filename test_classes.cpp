#include "classes.hpp"
#include <iostream>

void test_operator(){
    vec a(1,2,3);
    vec b(1,1,1);
    std::cout << a+b << std::endl;
    std::cout << a-b << std::endl;
    std::cout << a*2 << std::endl;
    std::cout << b/2 << std::endl;
    std::cout << b/0 << std::endl;
    std::cout << a*b << std::endl;
    std::cout << a.prod_vect(b) << std::endl;
}

int main(){
    //test_operator();
    return 0;
}