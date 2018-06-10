#include "vector.hpp"
#include <iostream>
//#include "commonenum.hpp"
using pathtracing::Vector3;

int main(){
    Vector3 a = Vector3(0,0,-1);
    Vector3 o = Vector3(0,0,0);
    Vector3 result = pathtracing::Rotate(pathtracing::Oriented_axis::X_axis, 3.1415926/4, o, a );
    std::cout << result.m_x << " " << result.m_y << " " << result.m_z << std::endl;
    return 0;
}