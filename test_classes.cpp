#include "vec.hpp"
#include "objects.hpp"
#include <iostream>

void test_operator(){
    vec a(1,2,3);
    vec b(1,1,1);
    color r(1,0,0);
    color c(0,2,0);
    //std::cout << a+b << std::endl;
    //std::cout << a-b << std::endl;
    //std::cout << a*2 << std::endl;
    //std::cout << b/2 << std::endl;
    //std::cout << b/0 << std::endl;
    //std::cout << a*b << std::endl;
    //std::cout << a.prod_vect(b) << std::endl;
    std::cout << r + c << std::endl;
    std::cout << r*5.0 << std::endl;
}

void test_objects(){
    sphere s(point(0,0,0),4.0);
    scene sc;
    sc.add(&s);
    std::cout << dynamic_cast<sphere*>(sc.getList()[0])->getCenter() << std::endl;
}

int main(){
    //test_operator();
    //test_objects();
    return 0;
}