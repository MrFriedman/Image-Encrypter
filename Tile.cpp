// Implementation of Tile.h 
#include "Tile.h"
#include <iostream>

using namespace std;

namespace FRDDYL002
{
	FRDDYL002::Tile::Tile() : x(0), y(0), height(0), width(0), pixel_data(nullptr)
	{}
	
	FRDDYL002::Tile::Tile(int y, int x, int width, int height, unsigned char** image_data) : x(x), y(y), width(width), height(height), image_data(image_data)
	{
		this->pixel_data = new unsigned char*[height];
		for (int i = y; i <= y + height; ++i) 
		{
		    this->pixel_data[i - y] = new unsigned char[width];
		    for (int j = x; j <= x + width; ++j)
		    {
		        this->pixel_data[i - y][j - x] = image_data[i][j];
		    }
		}
	
	}	
	
	FRDDYL002::Tile::~Tile()
	{
		if (pixel_data != nullptr)
		{
			for (int i = 0; i < height; i++)
			{
				delete[] pixel_data[i];
			}
			delete[] pixel_data;
			pixel_data = nullptr;
		}
	}
	
	unsigned char** FRDDYL002::Tile::getPixels() const
	{
		return pixel_data;
	}
	
	unsigned char FRDDYL002::Tile::getPixel(int row, int col) const 
	{
		return this->pixel_data[row][col];
	}
	
	void FRDDYL002::Tile::makeBlank() 
	{
		if (pixel_data != nullptr)
		{
			for (int i = 0; i < height; ++i)
			{
			    for (int j = 0; j < width; ++j) 
			    {
				pixel_data[i][j] = 0; // Set to blank value
			    }
		}
		}
	}
	
	int FRDDYL002::Tile::getX() const
    	{
        	return x;
    	}
    
    	int FRDDYL002::Tile::getY() const
    	{
       		return y;
    	}
	
	void FRDDYL002::Tile::swapTiles(Tile& swapping_tile)
	{
		swap(pixel_data, swapping_tile.pixel_data);
		swap(x, swapping_tile.x);
		swap(y, swapping_tile.y);
		swap(width, swapping_tile.width);
		swap(height, swapping_tile.height);
	}
}
