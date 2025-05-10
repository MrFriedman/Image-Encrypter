CC=g++

puzzle: PGMimage.o TileManager.o Tile.o main.o
	$(CC) PGMimage.o TileManager.o Tile.o main.o -o puzzle -std=c++11
	
 # Object file compilations
PGMImage.o: PGMImage.cpp
	$(CC) -c PGMImage.cpp -o PGMImage.o -std=c++11

TileManager.o: TileManager.cpp TileManager.h
	$(CC) -c TileManager.cpp -o TileManager.o -std=c++11
	
Til.o: Tile.cpp Tile.h
	$(CC) -c Tile.cpp -o Tile.o -std=c++11

main.o: main.cpp
	$(CC) -c main.cpp -o main.o -std=c++11
	
clean:
	rm -f *.o
	rm puzzle
