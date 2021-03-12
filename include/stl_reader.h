#pragma once
#ifndef STL_READER_H
#define STL_READER_H
#include <string>
#ifdef __cpplusplus
extern "C"{
#endif
double stl_surfacearea(std::string );
double stl_volume(std::string );
#ifdef __cpplusplus
}
#endif

#endif