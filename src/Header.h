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
	bool _isBinary;
	long int _size;
	uint8_t* _header;
	uint32_t _n_of_triangles;
	uint16_t _byte_count;
	Triangle* _T;
public:
	Data(){};
	Data(std::string );
	~Data(){};
	int get_triangles(std::string );
	void display_details();
	
};
