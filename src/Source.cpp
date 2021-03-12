#pragma once
#include<fstream>
#include<string.h>
#include<math.h>
#include "Header.h"

#define F *(float*)

Data::Data(std::string filename)
{
	std::fstream f;
	f.open(filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!f)
	{
		std::cout << "File not found!!" << std::endl;
		_size=0;
		_n_of_triangles=0;
		_isBinary=false;
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

			//std::cout << "$Constructor Invoked Successfully " << std::endl;

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
		//std::cout << "File not found!!" << std::endl;
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
		
		//std::cout<<"Processed "<<k<<" Triangles"<<std::endl;

		f1.close();
		delete[] buffer;
		ret=1;
	}
	else
	{
		ret=-2;
	}
	/*
	if (ret>0)
	{
		std::cout<<"get_triangles : successfully executed,return code : "<<ret<<std::endl;
	}
	*/
	return ret;
}

double Data::_signedVolofTriangle(Triangle & T)
{
	double v321 = T.vertex[2].x * T.vertex[1].y * T.vertex[0].z;
    double v231 = T.vertex[1].x * T.vertex[2].y * T.vertex[0].z;
    double v312 = T.vertex[2].x * T.vertex[0].y * T.vertex[1].z;
    double v132 = T.vertex[0].x * T.vertex[2].y * T.vertex[1].z;
    double v213 = T.vertex[1].x * T.vertex[0].y * T.vertex[2].z;
    double v123 = T.vertex[0].x * T.vertex[1].y * T.vertex[2].z;

    return (1.0f/6.0f)*(-v321 + v231 + v312 - v132 - v213 + v123);
}

double Data::_areaofTriangle(Triangle & T)
{
    double ax = T.vertex[1].x - T.vertex[0].x;
    double ay = T.vertex[1].y - T.vertex[0].y;
    double az = T.vertex[1].z - T.vertex[0].z;
    double bx = T.vertex[2].x - T.vertex[0].x;
    double by = T.vertex[2].y - T.vertex[0].y;
    double bz = T.vertex[2].z - T.vertex[0].z;
    double cx = ay*bz - az*by;
    double cy = az*bx - ax*bz;
    double cz = ax*by - ay*bx;

    return 0.5 * sqrt(cx*cx + cy*cy + cz*cz); 
}

double Data::get_Volume()
{
	double volume=0.0;

	for(unsigned i=0;i<_n_of_triangles;++i)
	{
		volume+=_signedVolofTriangle(_T[i]);
	} 

	return volume;
}

double Data::get_SurfaceArea()
{
	double SurfaceArea=0.0;
	for(unsigned i=0;i<_n_of_triangles;++i)
	{
		SurfaceArea+=_areaofTriangle(_T[i]);
	}
	return SurfaceArea;
}

void Data::display_details()
{
	if(_isBinary)
	{
		std::cout << "Number of Triangles : " << _n_of_triangles << std::endl;
		std::cout << "size of File : " << _size << std::endl;
		std::cout << "Volume of Mesh : "<< get_Volume() <<std::endl;
		std::cout << "Surface Area of Mesh : "<< get_SurfaceArea() <<std::endl;
	}
	else
	{
		std::cout <<"File could not be processed !!"<<std::endl;
	}
	

}

double stl_surfacearea(std::string filename)
{
	int r;
    double surfacearea;
	Data file(filename);
	r=file.get_triangles(filename);
    if(r==1)
    {
	    surfacearea=file.get_SurfaceArea();
    }
    else{surfacearea=-1.0;}
    
	return surfacearea;
} 

double stl_volume(std::string filename)
{
	int r;
    double volume;
	Data file(filename);
	r=file.get_triangles(filename);
    if(r==1)
    {
	volume=file.get_Volume();
    }
    else{volume=-1;}
    
	return volume;
} 