// Header file for TileManager.cpp - main logit of the program
// Need to initialise variables
// Read pgm and initialise board
#ifndef __TILEMANAGER__
#define __TILEMANAGER__

#include <string>
#include <vector>

using namespace std;

namespace FRDDYL002 {
	
	class Tile;
	
	//class PGMimage;
	
	class TileManager {
			int grid_number;
			int number_moves;
			int width, height, width_new, height_new;
			string input_filename, output_filename;
			unsigned char** pixel_data; // Stores pixels read from input image
			//unsigned char** image_data;
			unsigned char** board_buffer;
			Tile*** board;
			//vector<PGMimage> saved_images;
			
		public:
			TileManager();
			
			TileManager(int grid_number, int number_moves, const string& input_filename, const string& output_filename);
			
			~TileManager();
			
			void readImage(const string& input_filename);
			
			void writeImage(unsigned char** updated_board, int width, int height, const string& output_filename);
			
			void createBoard();
			
			void startGame();
			
			pair<int, int> randomMove(int blankXpos, int blankYpos);
			
			void saveState(int move, string output_filename);
			
			//void outputSummaryImage(int moveNum, int padding, int spacing, const string &output_filename);
	
	};

}

#endif
