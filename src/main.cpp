
#include "stl_reader.h"
#include <dlfcn.h>
#include <iostream>

int main()
{
    std::string filename = "./sample_stl/Impeller1.stl";
    
    double surface_area=stl_surfacearea(filename);
    double volume =stl_volume(filename);

    std::cout<<"SA :"<<surface_area<<" Vol. : "<<volume<<std::endl;

}