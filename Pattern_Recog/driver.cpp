#include <iostream>
#include <string>
#include "PGMimageProcessor.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Default parameters
    int minSize = 1;
    int threshold = 128;
    bool printData = false;
    string outputFile;
    string inputFile;
    int minFilter = -1, maxFilter = -1;

    // Basic argument parsing
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-m" && i+1 < argc) {
            minSize = stoi(argv[++i]);
        }
        else if (arg == "-f" && i+2 < argc) {
            minFilter = stoi(argv[++i]);
            maxFilter = stoi(argv[++i]);
        }
        else if (arg == "-t" && i+1 < argc) {
            threshold = stoi(argv[++i]);
            if (threshold < 0 || threshold > 255) {
                cerr << "Threshold must be 0-255\n";
                return 1;
            }
        }
        else if (arg == "-p") {
            printData = true;
        }
        else if (arg == "-w" && i+1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg[0] != '-') {
            inputFile = arg;
        }
        else {
            cerr << "Usage: " << argv[0] << " [-m minsize] [-f min max] [-t threshold] [-p] [-w outfile] input.pgm\n";
            return 1;
        }
    }

    if (inputFile.empty()) {
        cerr << "Error: No input file specified\n";
        return 1;
    }

    // Process the image
    FRDDYL002::PGMimageProcessor processor(inputFile);
    processor.extractComponents(threshold, minSize);

    if (minFilter > 0 && maxFilter > 0) {
        processor.filterComponentsBySize(minFilter, maxFilter);
    }

    if (printData) {
        processor.setPrintStatus(true);
    }

    if (!outputFile.empty()) {
        processor.writeComponents(outputFile);
        cout << "Processing complete. Output saved to " << outputFile << endl;
    }

    return 0;
}


/*#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>
#include "PGMimageProcessor.h"

using namespace std;

int main(int argc, char *argv[]) {
    int minSize = 1;          // Default minimum component size
    int minFilter = -1, maxFilter = -1;
    int threshold = 128;      // Default threshold for detection
    bool printData = false;
    string outputFile = "";
    string inputFile = "";

    int opt;
    while ((opt = getopt(argc, argv, "m:f:t:pw:")) != -1) {
        switch (opt) {
            case 'm':
                minSize = stoi(optarg);
                break;
            case 'f':
                if (optind < argc - 1) { 
                    minFilter = stoi(argv[optind + 1]);
                    maxFilter = stoi(argv[optind + 2]);
                }
                break;
            case 't':
                threshold = stoi(optarg);
                if (threshold < 0 || threshold > 255) {
                    cerr << "Error: Threshold must be between 0 and 255.\n";
                    return 1;
                }
                break;
            case 'p':
                printData = true;
                break;
            case 'w':
                outputFile = optarg;
                break;
            default:
                cerr << "Usage: findcomp [options] <inputPGMfile>\n";
                return 1;
        }
    }

    // Ensure an input PGM file is provided
    if (optind < argc) {
        inputFile = argv[optind];
    } else {
        cerr << "Error: No input PGM file provided.\n";
        return 1;
    }

    // Process the PGM image
    FRDDYL002::PGMimageProcessor processor(inputFile);
    
    // Note: Maybe rather pass all this info into the constructor then call a start method

    // Create connected components - extract connected comp (based on threshold); 
    // exlude comp less than minValidSize
    processor.extractComponents(threshold, minSize);

    // Apply filtering if specified
    if (minFilter > 0 && maxFilter > 0) {
        processor.filterComponentsBySize(minFilter, maxFilter);
    }

    // Print component information if requested
    processor.setPrintStatus(printData);
    

    // Save filtered image if requested
    if (!outputFile.empty()) {
        processor.writeComponents(outputFile);
        cout << "Meow done" << endl;
    } 

    return 0;
}
*/