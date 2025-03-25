// Main implementation file
// Will take in arg and instantiate TileManager

#include "TileManager.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

bool isOdd(int num) {
	return (num > 0 && num % 2 == 1);
}

int main(int argc, char *argv[]) {
	int grid_number, number_moves;
	string input_filename;
	string output_filename;
	
	// Check to see if the command line input was correct
	if (argc == 8) {
		for (int i = 1; i < argc; i++) {
			string arg = argv[i];
			if (arg == "-s") {
				grid_number = atoi(argv[i+1]);
				if (!isOdd(grid_number)) {
					cout << "Grid size/number must be odd" << endl;
					return 1;
				}
			}
			else if (arg == "-o") {
				output_filename = argv[i+1];
			}
			else if (arg == "-n") {
				number_moves = atoi(argv[i+1]);
			}
		}
		input_filename = argv[argc - 1];
	} else {
		grid_number = 3;
		number_moves = 3;
		input_filename = "mona512.pgm";
		output_filename = "test";
	}		
	
	// This is where TileManager get instantiated
	FRDDYL002::TileManager tileManager(grid_number, number_moves, input_filename, output_filename);
	
	// Method to call the startGame method which starts the game
	//tileManager.startGame();
	
    	return 0;
}
