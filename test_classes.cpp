#include "vec.hpp"
#include "objects.hpp"
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

void test_objects(){
    sphere s;
    scene sc;
    sc.list_objects.push_back(&s);
    sc.list_objects[0]->hit_object();
}

int main(){
    //test_operator();
    //test_objects();
    return 0;
}