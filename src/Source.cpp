#include<fstream>
#include "Header.h"

#define F *(float*)
Data::~Data()
{
	delete[] _header;
	delete[] T;
}
Data::Data(std::string &filename)
{
	std::fstream f;
	f.open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!f)
	{
		std::cout << "File not found!!" << std::endl;
	}
	else
	{
		std::filebuf* curser = f.rdbuf();   // Dont know what the hell this is 

		// pointer to the buffer menory

		auto size = curser->pubseekoff(0, f.end);

		_size = size;
		// sets the curser to bigining of the file

		curser->pubseekpos(0);

		// allocate memory to contain file data

		char* memory = new char[(size_t)size];

		curser->sgetn(memory, size);

		//Test to see if the file is binary
		/**/
		//_header = new uint8_t[80];
		char* memptr = memory;
		/*for (unsigned i = 0; i < 80; i++)
		{
			memptr += i;
			_header[i] = *(uint8_t*)memptr;
		}*/

		memptr += 80;
		_n_of_triangles =*(uint32_t*)(memptr);
		//std::cout << "Number of Triangles : " << _n_of_triangles;
		T = new Triangle[_n_of_triangles];
		
	}
}
int Data::read_stl(std::string &filename)
{
	std::fstream f;
	f.open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!f)
	{
		std::cout << "File not found!!" << std::endl;
	}
	else
	{
		std::filebuf* curser = f.rdbuf();   // Dont know what the hell this is 
		
		// pointer to the buffer menory

		auto size = curser->pubseekoff(0, f.end); 
		
		// sets the curser to bigining of the file

		curser->pubseekpos(0);

		// allocate memory to contain file data

		char* memory = new char[(size_t)size];

		curser->sgetn(memory, size);

		//Test to see if the file is binary
		/**/

		char* memptr = memory;
		memptr += 84;

		while (memptr < memory + size)
		{
			T->Normal.x = F(memptr);
			T->Normal.y = F(memptr + 4);
			T->Normal.z = F(memptr + 8);
			memptr += 12;
			for (unsigned i = 0; i < 3; i++)
			{
				T->vertex[i].x = F(memptr);
				T->vertex[i].y = F(memptr + 4);
				T->vertex[i].z = F(memptr + 8);
				memptr += 12;
			}
		}
	}
	return 0;
}
void Data::display_details(std::string &filename)
{

	std::cout << "Number of Triangles : " << _n_of_triangles;
	std::cout << "size of File : " << _size;

}

int main()
{
	std::string filename = "sample.stl";

	Data file(filename);
	file.read_stl(filename);
	file.display_details(filename);
	return 0;
}