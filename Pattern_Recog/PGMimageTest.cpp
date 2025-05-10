#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

namespace FRDDYL002 
{
    // Default contructor
    PGMimageProcessor::PGMimageProcessor() : imageData(nullptr), width(0), height(0), components() {}

    PGMimageProcessor::PGMimageProcessor(const std::string& inputFile) : imageData(nullptr), width(0), height(0), components()
    {
        loadImage(inputFile);
    }

    // Copy Constructor - start with a empty container
    PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor& rhs)
    : width(rhs.width), height(rhs.height), components() 
    {
        // Copy image data
        if (rhs.imageData != nullptr) {
            size_t dataSize = width * height;
            imageData = new unsigned char*[dataSize];
            copy(rhs.imageData, rhs.imageData + dataSize, imageData);
        } else {
            imageData = nullptr;
        }
    }

    // Move Constructor
    PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& rhs) : imageData(rhs.imageData),
    width(rhs.width), height(rhs.height), components(std::move(rhs.components))
    {
        // Reset the moved-from object
        rhs.imageData = nullptr;
        rhs.width = 0;
        rhs.height = 0;
        // components already moved
    }

    // Copy Assignment Operator
    PGMimageProcessor& PGMimageProcessor::operator=(const PGMimageProcessor& rhs) 
    {
        if (this != &rhs) { // Protect against self-assignment
            // Free existing resources
            delete[] imageData;
            
            // Copy data from rhs
            width = rhs.width;
            height = rhs.height;
            
            if (rhs.imageData != nullptr) {
                size_t dataSize = width * height;
                imageData = new unsigned char[dataSize];
                copy(rhs.imageData, rhs.imageData + dataSize, imageData);
            } else {
                imageData = nullptr;
            }
            
            // Clear and deep copy the components
            components.clear();
            for (const auto& component : rhs.components) {
                components.push_back(make_shared<ConnectedComponent>(*component));
            }
        }
        return *this;
    }

    // Move Assignment Operator
    PGMimageProcessor& PGMimageProcessor::operator=(PGMimageProcessor&& rhs) 
    {
        if (this != &rhs) { // Protect against self-assignment
            // Free existing resources
            delete[] imageData;
            
            // Move resources from other
            imageData = rhs.imageData;
            width = rhs.width;
            height = rhs.height;
            components = move(rhs.components);
            
            // Reset other
            rhs.imageData = nullptr;
            rhs.width = 0;
            rhs.height = 0;
            // components already moved
        }
        return *this;
    }
    
    // Destructor
    PGMimageProcessor::~PGMimageProcessor() 
    {
        delete[] imageData;
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
            cerr << "Failed top open file for read: " << fileName << endl;
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

    void FRDDYL002::PGMimageProcessor::write(const string& fileName)
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
    }

    // floodfill algorithm - check all possible 4-connected neighbours (N/S/E/W) and if a neighbour is 255, and not yet processed, 
    // add it to your component, pushing it’s non-tested N/S/E/W neighbour coordinates onto a queue (initially empty).
    void PGMimageProcessor::floodFill(int x, int y, int label, ConnectedComponent& comp) {
        std::queue<std::pair<int, int>> queue;
        queue.push({x, y});
        imageData[y][x] = label; // For 2D array use: imageData[y][x] = label;
    
        while (!queue.empty()) {
            auto [cx, cy] = queue.front();
            queue.pop();
            comp.addPixel(cx, cy);
    
            // Only check 4 directions (N/S/E/W)
            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};
            
            for (int i = 0; i < 4; ++i) {
                int nx = cx + dx[i];
                int ny = cy + dy[i];
                if (nx >= 0 && ny >= 0 && nx < width && ny < height && imageData[ny][nx] == 255) {
                    // Mark as processed
                    imageData[ny][nx] = label;
                    queue.push({nx, ny});
                }
            }
        }
    }

    int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
        components.clear();
        int componentID = 0;
        
        // Create a temporary thresholded image
        std::vector<unsigned char> thresholded(width * height);
        for (int i = 0; i < width * height; i++) {
            thresholded[i] = (buffer[i] >= threshold) ? 255 : 0;
        }
        
        // Find connected components
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (thresholded[y * width + x] == 255) {
                    auto component = std::make_shared<ConnectedComponent>(componentID++);
                    floodFill(x, y, label++, *component);
                    if (component->size() >= minValidSize) {
                        components.push_back(component);
                    }
                }
            }
        }
        return components.size();
    }

    int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize) {
        std::vector<ConnectedComponent> filteredComponents;
        for (const auto& comp : components) {
            if (comp.size() >= minSize && comp.size() <= maxSize) {
                filteredComponents.push_back(comp);
            }
        }
        components = std::move(filteredComponents);
        return components.size();
    }

    bool PGMimageProcessor::writeComponents(const std::string& outFileName) {
        std::ofstream file(outFileName);
        if (!file) {
            std::cerr << "Error: Cannot write components to " << outFileName << std::endl;
            return false;
        }

        std::vector<std::vector<int>> outputImage(height, std::vector<int>(width, 0));

        for (const auto& comp : components) {
            for (const auto& pixel : comp.getPixels()) {
                outputImage[pixel.second][pixel.first] = 255;
            }
        }

        file << "P2\n" << width << " " << height << "\n255\n";
        for (const auto& row : outputImage) {
            for (const auto& pixel : row) {
                file << pixel << " ";
            }
            file << "\n";
        }

        file.close();
        return true;
    }

    int PGMimageProcessor::getComponentCount() const {
        return components.size();
    }

    int PGMimageProcessor::getLargestSize() const {
        if (components.empty()) return 0;
        return std::max_element(components.begin(), components.end(),
                                [](const ConnectedComponent& a, const ConnectedComponent& b) { return a.size() < b.size(); })->size();
    }

    int PGMimageProcessor::getSmallestSize() const {
        if (components.empty()) return 0;
        return std::min_element(components.begin(), components.end(),
                                [](const ConnectedComponent& a, const ConnectedComponent& b) { return a.size() < b.size(); })->size();
    }

    void PGMimageProcessor::printComponentData(const ConnectedComponent& theComponent) const {
        int componentIndex = std::distance(components.begin(), std::find(components.begin(), components.end(), theComponent));
        std::cout << "Component ID: " << (componentIndex + 1)
                  << ", Size: " << theComponent.size() << " pixels\n";
    }

}
