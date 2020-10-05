#pragma once
#include <iostream>

struct Point
{
	float x, y, z;
};

struct Triangle
{
	Point Normal;
	Point vertex[3];
};

class Data
{
	long int _size;
	uint8_t* _header;
	uint32_t _n_of_triangles;
	uint16_t _byte_count;
	Triangle* T;
public:
	Data(std::string &filename);
	~Data();
	int read_stl(std::string &);
	void display_details(std::string&);
	
};