// Main logit of the program
#include "TileManager.h"
#include "Tile.h"
#include "PGMimage.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

namespace FRDDYL002 
{
	// Default Constructor
	FRDDYL002::TileManager::TileManager() : grid_number(0), last_grid_height(0), last_grid_width(0), number_moves(0), input_filename(""), output_filename("")
	{}
	
	// Custom Constructor - reads image and creates board
	FRDDYL002::TileManager::TileManager(int grid_number, int last_grid_height, int last_grid_width, int number_moves, const std::string& input_filename, const std::string& output_filename) : 
	grid_number(grid_number), last_grid_height(last_grid_height), last_grid_width(last_grid_width), number_moves(number_moves), input_filename(input_filename), output_filename(output_filename) 
	{	
		readImage(input_filename);
		createBoard();
	}
	
	// Destructer
	FRDDYL002::TileManager::~TileManager()
	{
		if (board != nullptr)
		{
			for (int i = 0; i < grid_number; i++) 
			{
				if (board[i] != nullptr) 
				{
					delete[] board[i];
					board[i] = nullptr;
				}
			}
			delete[] board;
			board = nullptr;
		}
		cout << "Nlr" << endl;
		/*
		if (last_grid != nullptr)
		{

			for (int i = 0; i < last_grid_height; i++) 
			{
				if (last_grid[i] != nullptr) 
				{
					delete[] last_grid[i];
					last_grid[i] = nullptr;
				}
			}
			delete[] last_grid;
			last_grid = nullptr;
		}
		*/
		
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
	
	// Read in PGM and create 2D array of pixel_data
	void FRDDYL002::TileManager::readImage(const string& input_filename) 
	{
		/// Read in the Image and create a 2D array for the pixel data
		PGMimage pgm_reader;
		pgm_reader.read(input_filename);
		
		pgm_reader.getDims(width, height);
		const unsigned char* image_data = pgm_reader.getBuffer();
		
		TileManager::pixel_data = new unsigned char*[height];
		for (int i = 0; i < height; i++) {
			TileManager::pixel_data[i] = new unsigned char[width];
			for (int j = 0; j < width; j++) {
				TileManager::pixel_data[i][j] = image_data[i * width + j];
				//cout << "Pixel[" << i << "][" << j << "] = " 
			          //<< static_cast<int>(TileManager::pixel_data[i][j]) << std::endl;
			}
		}
		
	}

	// Take in the new board after a move is performed
	void FRDDYL002::TileManager::writeImage(unsigned char** updated_board, int width, int height, const string& output_filename) 
	{
		// Using ofstream to write the output file
		ofstream outfile(output_filename);
		if (outfile.is_open())
		{
			// Added the comments, file size and limit for pixel number
			outfile << "P2" << endl;
			outfile << "# Output PGM File" << endl;
			outfile << width << " " << height << endl;
			outfile << "255" << endl;
			
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					unsigned char pixel = updated_board[i][j];
					outfile << static_cast<int>(pixel) << " ";
				}
				outfile << "\n";
			}

			outfile.close();
			
			// Showing the correct file out put name
			cout << "Saved current state to: " << output_filename << endl;
		}
	}
	
	// Create board of Tiles and instantiate Tile objects in board
	void FRDDYL002::TileManager::createBoard() 
	{
		// Need to divide the width and height up.
		int tile_width = int(width / grid_number);
		int tile_height = int(height / grid_number);

		// Need to divide the width and height *of the last grid up.
		int last_tile_width = int(width / last_grid_width);
		int last_tile_height = int(height / last_grid_height);
		
		// Recalculate width and height
		int width_new = tile_width * grid_number;
		int height_new = tile_height * grid_number;
		cout << "Tile width: " << tile_width << endl;

		/* Recalculate width and height but considering the last_grid??????
		int width_new = tile_width * grid_number;
		int height_new = tile_height * grid_number; */
		
		board = new Tile**[grid_number]; // 2D array of type Tiles with rows = grid_size
		//last_grid = new Tile**[last_grid_height];
		// Initialise board
		for (int i = 0; i < grid_number; i++) 
		{
			board[i] = new Tile*[grid_number]; // Every index of board should have a column = grid_size
		}
		/* Initialise last grid
		for (int j = 0; j < last_grid_height; j++) 
		{
			last_grid[j] = new Tile*[last_grid_width]; // Every index of board should have a column = grid_size
		}
			*/
		// Pass pixel data for the board but for last section divide up for last grid
		for (int tile_y = 0; tile_y < height_new; tile_y+=tile_height)
		{
			for (int tile_x = 0; tile_x < width_new; tile_x+=tile_width) 
			{
				// if on the last grid
				//if (int(height_new/tile_height) == 2)
				//{
				//cout << "Last Tile" << endl;
				Tile* tile = new Tile(tile_y, tile_x, tile_width, tile_height, pixel_data);

				board[tile_y / tile_height][tile_x / tile_width] = tile;

				//Average Pixel Number of each tile
				cout << "Average Pixel Value for Tile at (" << tile_x << ", " << tile_y << "): " 
				<< tile->getavgPixel() << endl;
				//}
				//else { cout << "Poes" << endl; }
			}
			
		}
	
	}
	
	// n times do: random move of tiles; retrieve current poition of blank tile and save board
	void FRDDYL002::TileManager::startGame() 
	{
		int blankXpos, blankYpos;
		blankXpos = grid_number - 1;
		blankYpos = grid_number - 1;
		for (int move = 1; move <= number_moves; ++move) 
		{
			// Need to do a random tile swap
			pair<int, int> blankPos = randomMove(blankXpos, blankYpos);
			blankXpos = blankPos.first;
			blankYpos = blankPos.second;
			saveState(move, output_filename);
			
		}
	}
	
	// Find the available moves for the blank tile and randomly select one
	pair<int, int> FRDDYL002::TileManager::randomMove(int blankXpos, int blankYpos)
	{
		std::vector<std::pair<int, int>> possibleMoves; // Store valid moves

		// Current blank tile position in the grid
		int i = blankXpos;
		int j = blankYpos;

		// Check Right Move (j + 1)
		if (j + 1 < grid_number) {
		    possibleMoves.push_back({i, j + 1});
		}

		// Check Left Move (j - 1)
		if (j - 1 >= 0) {
		    possibleMoves.push_back({i, j - 1});
		}

		// Check Down Move (i + 1)
		if (i + 1 < grid_number) {
		    possibleMoves.push_back({i + 1, j});
		}

		// Check Up Move (i - 1)
		if (i - 1 >= 0) {
		    possibleMoves.push_back({i - 1, j});
		}


		// Ensure there is at least one valid move
		if (possibleMoves.empty())
		{
			return {blankXpos, blankYpos}; // No move possible (should not happen)
		}

		// Pick a random valid move
		random_device rd;   // Random seed
		mt19937 g(rd());    // Random number generator

		// Shuffle possible moves to ensure randomness
		shuffle(possibleMoves.begin(), possibleMoves.end(), g);

		// Pick the first move in the shuffled list
		pair<int, int> chosenMove = possibleMoves[0];


		// Swap the blank tile with the chosen tile
		board[blankXpos][blankYpos]->swapTiles(*board[chosenMove.first][chosenMove.second]);

		// Return the new blank tile position
		return chosenMove;
	}

	//
	void FRDDYL002::TileManager::saveState(int move, string output_filename)
	{
	    	int tile_width = int(width / grid_number);
		int tile_height = int(height / grid_number);
		
		// Recalculate width and height
		int width_new = tile_width * grid_number;
		int height_new = tile_height * grid_number;

	    	// Allocate memory for the new board
	    	board_buffer = new unsigned char*[height_new];
	    	for (int i = 0; i < height_new; i++)
	    	{
			board_buffer[i] = new unsigned char[width_new];
			
	    	}

	    	// Iterate over each tile in the grid - Ganna need to iterate over the last 27...
	    	for (int tile_y = 0; tile_y < height_new; tile_y += tile_height)
		{
			for (int tile_x = 0; tile_x < width_new; tile_x += tile_width) 
			{
				if (tile_y < height_new * grid_number && tile_x < width_new * grid_number)
				{
					int tile_row = tile_y / tile_height;
		        		int tile_col = tile_x / tile_width;
		        		Tile* tile = board[tile_row][tile_col]; // Get tile from the board
		        
		        		if (tile != nullptr)
		        		{
		            			for (int row = 0; row < tile_height; row++)
		            			{
		                			for (int col = 0; col < tile_width; col++)
		                			{
										unsigned char tile_pixel = tile->getPixel(row, col);
										board_buffer[tile_y + row][tile_x + col] = tile_pixel;
		                			}
		            			}
		        		}
				}
			}
		}

		writeImage(board_buffer, width_new, height_new, output_filename + "-" + to_string(move) + ".pgm");
		
		for (int i = 0; i < height_new; i++)
    		{
			delete[] board_buffer[i];
    		}
    		delete[] board_buffer;
    		board_buffer = nullptr;

	}	
	
}
