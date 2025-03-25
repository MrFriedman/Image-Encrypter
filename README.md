The aim of this assignment is to implement a tile sliding puzzle where an image (that the user can input or leave for default image file selection) is divided into a grid of tiles. The tiles that are available for moving are chosen randomly and the program saves each move that is done, to a ouput file (that the user can input or leave for default image file selection).

main.cpp:
	This is the entry point of the program where the command-line arguments are handled and a TileManager object is created to start the game.
	
TileManager.cpp & TileManager.h
	This is the implementation file and the header file, respectivly, where the input is read and divided up into a 2D array of Tile objects; game save state is located here as well as the random move and write to file functions. 
	
Tile.cpp & Tile.h:
	This is the implementation file and the header file, respectivly, where memory allocation for the pixel data is implemented as well as tile data getters, tile swapping and making the tile blank legic is stored.
	
PGMimage.cpp & PGMimage.h has been provided by the lecturer for usage.

How to run the program:
	- Open to the programs directory in your terminal
	- Run "make"
	- Run "./puzzle -s <grid_number> -o <output_filename> -n <number_moves> <input_image>"
	- If you want to re-run; make sure to run "make clean"
