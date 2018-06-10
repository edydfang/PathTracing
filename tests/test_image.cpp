#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"

int main(){
    int x,y,n;
    unsigned char *data = stbi_load("ARU.jpg", &x, &y, &n, 3);
    std::cout << x << " " << y << " " << std::endl;
    std::cout << (short)data[1] << std::endl;
    stbi_image_free(data);
}