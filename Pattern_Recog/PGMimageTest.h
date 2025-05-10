#ifndef PGMIMAGEPROCESSOR_H
#define PGMIMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include <vector>
#include <string>
#include <set>
#include <memory>

using namespace std;

namespace FRDDYL002 {

    class PGMimageProcessor {
    private:
        struct Component {
            set<pair<int, int>> pixels;
            int size() const { return pixels.size(); }
        };

        int width;                // Width of the image
        int height;               // Height of the image
        unsigned char* buffer;    // Buffer to hold image data
        int maxGrayValue;         // Maximum gray value in the PGM file
        string inputFile;         
        unsigned char** imageData; // 2D vector to store pixel data
        vector<shared_ptr<ConnectedComponent>> components; // Detected components

        void floodFill(int x, int y, int label, ConnectedComponent& comp);

    public:
        // Default Constructor
        PGMimageProcessor();

        // Parameterised Constructor
        PGMimageProcessor(const std::string& inputFile);

        // Copy constructor
        PGMimageProcessor(const PGMimageProcessor & rhs);

        // Move constructor
        PGMimageProcessor(PGMimageProcessor && rhs);

        // Copy and Move Assignment Operators
        PGMimageProcessor & operator=(const PGMimageProcessor & rhs);

        PGMimageProcessor & operator=(PGMimageProcessor && rhs);

        // Destructor
        ~PGMimageProcessor();

        void loadImage(const string& input_filename);

        // getters
        const unsigned char* getBuffer(void) const
        {
            return buffer;
        }

        void getDims(int& wd, int& ht) const
        {
            wd = width; ht = height;
        }

        // set Image data from existing sized buffer (1D flattened array)
        void setImageData(unsigned char* data, int wd, int ht);

        // read and write PGM images
        void read(const std::string& fileName);
        void write(const std::string& fileName);

        // Apply thresholding to the image
        void applyThreshold(int threshold);

        /* process the input image to extract all the connected components,
        based on the supplied threshold (0...255) and excluding any components
        of less than the minValidSize. The final number of components that
        you store in your container (after discarding undersized one)
        must be returned.
        */
        int extractComponents(unsigned char threshold, int minValidSize);

        /* iterate - with an iterator - though your container of connected
        components and filter out (remove) all the components which do not
        obey the size criteria, [minSize, maxSiz e ] passed as
        arguments. The number remaining after this operation should be
        returned. NOTE: minSize need not be the minValidSize above – you can
        choose to keep ANY range of sizes after initial processing. These are
        logically distinct operations and a user of your API may want this.
        */
        int filterComponentsBySize(int minSize, int maxSize);

        /* create a new PGM file which contains all available components
        (255=component pixel, 0 otherwise) and write this to outFileName as a
        valid PGM. the return value indicates success of operation
        */
        bool writeComponents(const std::string & outFileName);

        // return number of components
        int getComponentCount(void) const;

        // return number of pixels in largest component
        int getLargestSize(void) const;

        // return number of pixels in smallest component
        int getSmallestSize(void) const;

        /* print the data for a component to std::cout
        see ConnectedComponent class;
        print out to std::cout: component ID, number of pixels
        */
        void printComponentData(const ConnectedComponent & theComponent) const;
    };

}

#endif // PGMIMAGEPROCESSOR_H