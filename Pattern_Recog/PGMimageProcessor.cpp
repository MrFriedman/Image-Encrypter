#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <algorithm>
#include <stack>
#include <utility>

using namespace std;

namespace FRDDYL002 
{
    // Default Constructor
    PGMimageProcessor::PGMimageProcessor() : width(0), height(0), buffer(nullptr), maxGrayValue(0), 
    inputFile(""), imageData(nullptr) 
    {}
    
    PGMimageProcessor::PGMimageProcessor(const std::string& inputFile) : width(0), height(0), buffer(nullptr), maxGrayValue(0), 
    inputFile(inputFile), imageData(nullptr) 
    {
        loadImage(inputFile);
    }

    // Copy constructor
    PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & rhs) :
        width(rhs.width), height(rhs.height), maxGrayValue(rhs.maxGrayValue),
        inputFile(rhs.inputFile), components(rhs.components) {
        
        // Deep copy the buffer
        if(rhs.buffer != nullptr) {
            buffer = new unsigned char[width * height];
            for (int i = 0; i < width * height; ++i) {
                buffer[i] = rhs.buffer[i];
            }
        } else {
            buffer = nullptr;
        }
        
        // Deep copy the 2D image data
        if(rhs.imageData != nullptr) {
            imageData = new unsigned char*[height];
            for(int i = 0; i < height; ++i) {
                imageData[i] = new unsigned char[width];
                if(rhs.imageData[i] != nullptr) {
                    for (int j = 0; j < width; ++j) {
                        imageData[i][j] = rhs.imageData[i][j];
                    }
                }
            }
        } else {
            imageData = nullptr;
        }
    }

    // Move Constructor
    PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && rhs) : width(rhs.width), 
    height(rhs.height), buffer(rhs.buffer), maxGrayValue(rhs.maxGrayValue), inputFile(std::move(rhs.inputFile)), 
    imageData(rhs.imageData), components(std::move(rhs.components)) 
    {
        // Set rhs members to null state to prevent double deletion
        rhs.width = 0;
        rhs.height = 0;
        rhs.buffer = nullptr;
        rhs.maxGrayValue = 0;
        rhs.imageData = nullptr;
        // components will be moved automatically since it's using std::move
    }

    // Copy Assignment Operator
    PGMimageProcessor & PGMimageProcessor::operator=(const PGMimageProcessor & rhs) {
        if(this != &rhs) { // Avoid self-assignment
            // Clean up existing resources
            if(buffer != nullptr) {
                delete[] buffer;
                buffer = nullptr;
            }
            
            if(imageData != nullptr) {
                for(int i = 0; i < height; ++i) {
                    if(imageData[i] != nullptr) {
                        delete[] imageData[i];
                    }
                }
                delete[] imageData;
                imageData = nullptr;
            }
            
            // Copy the simple members
            width = rhs.width;
            height = rhs.height;
            maxGrayValue = rhs.maxGrayValue;
            inputFile = rhs.inputFile;
            
            // Deep copy the buffer
            if(rhs.buffer != nullptr) {
                buffer = new unsigned char[width * height];
                for (int i = 0; i < width * height; ++i) {
                    buffer[i] = rhs.buffer[i];
                }
            }
            
            // Deep copy the 2D image data
            if(rhs.imageData != nullptr) {
                imageData = new unsigned char*[height];
                for(int i = 0; i < height; ++i) {
                    imageData[i] = new unsigned char[width];
                    if(rhs.imageData[i] != nullptr) {
                        for (int j = 0; j < width; ++j) {
                            imageData[i][j] = rhs.imageData[i][j];
                        }
                    }
                }
            }
            
            // Copy the components
            components = rhs.components;
        }
        return *this;
    }

    // Move Assignment Operator
    PGMimageProcessor & PGMimageProcessor::operator=(PGMimageProcessor && rhs) {
        if(this != &rhs) { // Avoid self-assignment
            // Clean up existing resources
            if(buffer != nullptr) {
                delete[] buffer;
            }
            
            if(imageData != nullptr) {
                for(int i = 0; i < height; ++i) {
                    if(imageData[i] != nullptr) {
                        delete[] imageData[i];
                    }
                }
                delete[] imageData;
            }
            
            // Move the simple members
            width = rhs.width;
            height = rhs.height;
            maxGrayValue = rhs.maxGrayValue;
            inputFile = std::move(rhs.inputFile);
            
            // Move the pointers
            buffer = rhs.buffer;
            imageData = rhs.imageData;
            
            // Move the components
            components = std::move(rhs.components);
            
            // Set rhs members to null state to prevent double deletion
            rhs.width = 0;
            rhs.height = 0;
            rhs.buffer = nullptr;
            rhs.maxGrayValue = 0;
            rhs.imageData = nullptr;
            // components will be moved automatically with std::move
        }
        return *this;
    }
    
    // Destructor
    PGMimageProcessor::~PGMimageProcessor() {
        // Free the buffer
        if(buffer != nullptr) {
            delete[] buffer;
            buffer = nullptr;
        }
        
        // Free the 2D image data
        if(imageData != nullptr) {
            for(int i = 0; i < height; ++i) {
                if(imageData[i] != nullptr) {
                    delete[] imageData[i];
                }
            }
            delete[] imageData;
            imageData = nullptr;
        }
        
        // No need to manually free components since we're using shared_ptr
    }    

    void PGMimageProcessor::loadImage(const string& inputFile)
    {
        // Read the PGM file
        // Allocate memory for imageData
        // Set width and height
        // Exception handling for file not found or invalid format
        read(inputFile);
        getDims(width, height);
        const unsigned char* imageBuffer = getBuffer();

        imageData = new unsigned char*[height];
        for (int i = 0; i < height; i++){
            imageData[i] = new unsigned char[width];
            for (int j = 0; j < width; j++){
                imageData[i][j] = buffer[i * width + j];   
            }
        }

    }

    void PGMimageProcessor::setImageData(unsigned char* data, int wd, int ht)
    {
        if (data == nullptr || wd < 1 || ht < 1)
        {
            cerr << "setImageData() invalid data specified - aborted.\n";
            return;
        }
        if (buffer) delete[] buffer;
        buffer = new unsigned char[wd * ht];
        width = wd; height = ht;
        for (size_t i = 0; i < wd * ht; ++i) buffer[i] = data[i];
    }

    // Read in PGM image and create 2D array of pixel_data
    void FRDDYL002::PGMimageProcessor::read(const string& fileName)
    {
        ifstream ifs(fileName, ios::binary);
        if (!ifs)
        {
            cerr << "Failed to open file for read: " << fileName << endl;
            return;
        }
        string line;
        ifs >> line >> ws;
        if (line != "P5")
        {
            cerr << "Malformed PGM file - magic is: " << line << endl;
            return;
        }
        while (getline(ifs, line))
        {
            //cout << line << endl;
            if (line[0] != '#') break;
        }
        istringstream iss(line);
        iss >> width >> height;
        //cout << "width, height = (" << width << "," << height << ")\n";

        if (!iss)
        {
            cerr << "Header not correct - unexpected image sizes found: " << line << endl;
            return;
        }
        int maxChan = 0;
        ifs >> maxChan >> ws;
        if (maxChan != 255)
        {
            cerr << "Max grey level incorect - found: " << maxChan << endl;
        }
        // start of binary block

        buffer = new unsigned char[width * height];
        ifs.read(reinterpret_cast<char*>(buffer), width * height);

        if (!ifs)
        {
            cerr << "Failed to read binary block - read\n";
        }

        ifs.close();
    }

    /*void FRDDYL002::PGMimageProcessor::write(const string& fileName)
    {
        if (buffer == nullptr || width < 1 || height < 1)
        {
            cerr << "Invalid data for PGM write to " << fileName << endl;
            return;
        }
        ofstream ofs(fileName, ios::binary);
        if (!ofs)
        {
            cerr << "Unable to open PGM output file " << fileName << endl;
            return;
        }

        ofs << "P5\n#File produced by P Marais\n" << width << " " << height << endl << 255 << endl;
        ofs.write(reinterpret_cast<char*>(buffer), width * height);
        if (!ofs)
        {
            cerr << "Error writing binary block of PGM.\n";
        }

        ofs.close();
    }*/

    int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
        components.clear();
        int componentID = 0;
        
        // Create a visited mask
        vector<bool> visited(width * height, false);

        //cout << "Starting component extraction (threshold: " << (int)threshold << ", min size: " << minValidSize << ")" << endl;
    
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Check if pixel is above the threshold -> need to implement new check for encryption
                /*
                    Note: Here we want to check if a pixel is blank
                */
                if (!visited[y*width + x] && imageData[y][x] >= threshold) {
                    //cout << "Found pixel above threshold at (" << x << "," << y << "): " << (int)imageData[y][x] << endl;

                    auto component = make_shared<ConnectedComponent>(componentID++);
                    floodFill(x, y, componentID, *component, visited, threshold);  
                    //cout << "GFr" << endl;
                    if (component->getSize() >= minValidSize) {
                        components.push_back(component);
                    }
                }
            }
        }
        return components.size();
    }
    
    // Modified floodFill:
    void PGMimageProcessor::floodFill(int x, int y, int label, ConnectedComponent& comp, std::vector<bool>& visited,
    unsigned char threshold) 
    {
        stack<pair<int,int>> pixels;
        pixels.push(make_pair(x,y));

        while (!pixels.empty()) {
            // C++11 compatible way to unpack pair
            int cx = pixels.top().first;
            int cy = pixels.top().second;
            pixels.pop();

            // Boundary checks
            if (cx < 0 || cy < 0 || cx >= width || cy >= height)
                continue;

            size_t index = cy*width + cx;
            if (visited[index] || imageData[cy][cx] < threshold)
                continue;

            visited[index] = true;
            imageData[cy][cx] = label;
            comp.addPixel(cx, cy);

            // Inside floodFill, after comp.addPixel(cx, cy);
            //cout << "MM: Added pixel (" << cx << ", " << cy << ") to component " << label << std::endl;

            // 4-way connectivity
            pixels.push(make_pair(cx+1, cy));
            pixels.push(make_pair(cx-1, cy));
            pixels.push(make_pair(cx, cy+1));
            pixels.push(make_pair(cx, cy-1));
        }
    }

    int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize) {
        vector<shared_ptr<ConnectedComponent>> filteredComponents;
        for (const auto& comp : components) {
            if (comp->getSize() >= minSize && comp->getSize() <= maxSize) {
                filteredComponents.push_back(comp);
            }
        }
        components = move(filteredComponents);
        return components.size();
    }

    bool PGMimageProcessor::writeComponents(const std::string& outFileName) {
        // Create a new buffer for the output image
        unsigned char* outputBuffer = new unsigned char[width * height];
        
        // Initialize to all black (0)
        for (int i = 0; i < width * height; i++) {
            outputBuffer[i] = 0;
        }
        
        // Set component pixels to white (255)
        for (const auto& comp : components) {
            for (const auto& pixel : comp->getPixels()) {
                int x = pixel.first;
                int y = pixel.second;
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    outputBuffer[y * width + x] = 255;
                }
            }
        }
        
        // Open file in binary mode
        std::ofstream file(outFileName, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Cannot write components to " << outFileName << std::endl;
            delete[] outputBuffer;
            return false;
        }
        
        // Write header
        file << "P5\n" << width << " " << height << "\n255\n";
        
        // Write binary data
        file.write(reinterpret_cast<char*>(outputBuffer), width * height);
        
        file.close();
        delete[] outputBuffer;
        return true;
    }

    int PGMimageProcessor::getComponentCount() const {
        return components.size();
    }

    int PGMimageProcessor::getLargestSize() const {
        if (components.empty()) return 0;
        
        int maxSize = components[0]->getSize();
        for (const auto& comp : components) {
            if (comp->getSize() > maxSize) {
                maxSize = comp->getSize();
            }
        }
        return maxSize;
    }

    int PGMimageProcessor::getSmallestSize() const {
        if (components.empty()) return 0;
        
        int minSize = components[0]->getSize();
        for (const auto& comp : components) {
            if (comp->getSize() < minSize) {
                minSize = comp->getSize();
            }
        }
        return minSize;
    }

    void PGMimageProcessor::setPrintStatus(bool printData){
        if (printData){
            cout << "Debug: Number of components = " << components.size() << endl; // Add this
            for (const auto& comp : components){
                //cout << "Meow" << endl;
                printComponentData(comp);
            }
        }
    }

    void PGMimageProcessor::printComponentData(const shared_ptr<ConnectedComponent>& theComponent) const {
        int componentIndex = distance(components.begin(), find(components.begin(), components.end(), theComponent));
        cout << "Component ID: " << (componentIndex + 1) << ", Size: " << theComponent->getSize() << " pixels\n";
    }
}
