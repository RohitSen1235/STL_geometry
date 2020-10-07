#include<fstream>
#include<string.h>
#include "Header.h"

#define F *(float*)

Data::Data(std::string filename)
{
	std::fstream f;
	f.open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!f)
	{
		std::cout << "File not found!!" << std::endl;
	}
	else
	{
		std::filebuf* cursor = f.rdbuf();   		

		// pointer to the buffer menory

		auto size = cursor->pubseekoff(0, f.end);

		_size = size;

		char* fw=new char[(size_t)(6)];
		
		// sets the cursor to bigining of the file
		
		cursor->pubseekpos(0);
		
		// Checking if file is binary or not 

		std::streampos pos=6;
		cursor->sgetn(fw,pos);
		//std::cout<<fw<<std::endl;		cursor->pubseekpos(0);

		char solid[]="solid\0";
		int a=strncmp(fw,solid,5);
		//std::cout<<a<<std::endl;
		if(a>=0)
		{
			_isBinary=false;
			std::cout<<"File NOT Binary!! "<<std::endl;
			_size=0;
			_n_of_triangles=0;
			exit(0);
		}
		else
		{
			cursor->pubseekpos(0);
			_isBinary=true;
			// allocate memory to contain file data
			char* memory = new char[(size_t)size];

			cursor->sgetn(memory, size);

			char* memptr = memory;

			memptr += 80; // skipped first 80 bytes - header 

			_n_of_triangles =*(uint32_t*)(memptr);

			std::cout << "$Constructor Invoked Successfully " << std::endl;

			delete[] memory;
		}
		
		f.close();
		
		delete[] fw;
	}
}

int Data::get_triangles(std::string filename)
{
	int ret;
	std::fstream f1;
	f1.open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!f1)
	{
		std::cout << "File not found!!" << std::endl;
		ret=-1;
	}
	else if(_isBinary)
	{
		std::filebuf* cursor = f1.rdbuf();   
		
		// pointer to the buffer menory

		auto size = cursor->pubseekoff(0, f1.end); 

		// sets the cursor to bigining of the file

		cursor->pubseekpos(0);

		// allocate memory to contain file data

		char* buffer = new char[(size_t)size];

		cursor->sgetn(buffer, size);

		char* memptr = buffer;

		memptr += 84;	//skipped first 84 bytes

		_T = new Triangle[_n_of_triangles];
		int k=0;
		while (memptr < buffer+size)
		{
			_T[k].Normal.x = F(memptr);
			_T[k].Normal.y = F(memptr + 4);
			_T[k].Normal.z = F(memptr + 8);
			memptr += 12;
			for (unsigned i = 0; i < 3; i++)
			{
				_T[k].vertex[i].x = F(memptr);
				_T[k].vertex[i].y = F(memptr + 4);
				_T[k].vertex[i].z = F(memptr + 8);
				memptr += 12;
			}
			memptr+=2;// skipping Atribubte byte count
			k++;
		}
		
		std::cout<<"Processed "<<k<<" Triangles"<<std::endl;

		f1.close();
		delete[] buffer;
		ret=1;
	}
	else
	{
		ret=-2;
	}
	
	if (ret>0)
	{
		std::cout<<"get_triangles : successfully executed return code : "<<ret<<std::endl;
	}
	
	return ret;
}

void Data::display_details()
{

	std::cout << "Number of Triangles : " << _n_of_triangles << std::endl;
	std::cout << "size of File : " << _size << std::endl;

}

int main()
{
	int r;
	//std::string filename = "../sample_stl/icosahedron.stl";
	std::string filename = "../sample_stl/sample2.stl";

	Data file(filename);
	r=file.get_triangles(filename);
	
	file.display_details();
	return 0;
} 