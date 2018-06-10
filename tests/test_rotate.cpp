#include "vector.hpp"
#include <iostream>
//#include "commonenum.hpp"
using smallpt::Vector3;

int main(){
    Vector3 a = Vector3(0,0,-1);
    Vector3 o = Vector3(0,0,0);
    Vector3 result = smallpt::Rotate(smallpt::Oriented_axis::X_axis, 3.1415926/4, o, a );
    std::cout << result.m_x << " " << result.m_y << " " << result.m_z << std::endl;
    return 0;
}