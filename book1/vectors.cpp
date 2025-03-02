#include <iostream>
#include "vec.h" // quotes bc this is a header we created

int main()
{
    vec3 a = vec3(1, 2, 3); // according to the constructor we have in the vec3 class
    vec3 b = vec3(4, 5, 6); // according to the constructor we have in the vec3 class

    std::cout << "the a vector is " << a << std::endl; // prints "the a vector is 1 2 3"
    std::cout << b << std::endl;                       // prints "4 5 6"

    std::cout << "a * b is " << a * b << std::endl;
}