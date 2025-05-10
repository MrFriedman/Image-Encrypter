CC = g++
CFLAGS = -std=c++11

findcomp: ConnectedComponent.o PGMimageProcessor.o driver.o
	$(CC) ConnectedComponent.o PGMimageProcessor.o driver.o -o findcomp $(CFLAGS)

# Compile source files into object files
ConnectedComponent.o: ConnectedComponent.cpp ConnectedComponent.h
	$(CC) -c ConnectedComponent.cpp -o ConnectedComponent.o $(CFLAGS)

PGMimageProcessor.o: PGMimageProcessor.cpp PGMimageProcessor.h
	$(CC) -c PGMimageProcessor.cpp -o PGMimageProcessor.o $(CFLAGS)

driver.o: driver.cpp 
	$(CC) -c driver.cpp -o driver.o $(CFLAGS)

# Test executable
runtests: driver_tester.o PGMimageProcessor.o ConnectedComponent.o
	$(CC) driver_tester.o PGMimageProcessor.o ConnectedComponent.o -o runtests $(CFLAGS)

driver_tester.o: driver_tester.cpp 
	$(CC) -c driver_tester.cpp -o driver_tester.o $(CFLAGS)

# Run tests
tests: runtests
	./runtests

# Clean up object files and executable
clean:
	rm -f *.o
	rm findcomp runtests