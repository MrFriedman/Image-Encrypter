// Header file for Tile Object file
// Define the variables of the Tile object (constructors; getters; settersBig 7 must be defined)
#ifndef __TILE__
#define __TILE__

namespace FRDDYL002
{
	class Tile {
		int x;
		int y;
		int height;
		int width;
		unsigned char** pixel_data;
		unsigned char** image_data;
		
		public:
		Tile();
		
		// Custom Constructor - x & y are pixel coordinates
		Tile(int y, int x, int height, int width, unsigned char** image_data);
		
		~Tile();
		
		// Need getters and setters
		unsigned char** getPixels() const;
		
		unsigned char getPixel(int row, int col) const;
		
		int getavgPixel();

		int getX() const;
		
		int getY() const;
		
		void swapTiles(Tile& swapping_tile);
	};
}

#endif
