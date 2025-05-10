#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function
#include "catch.hpp"
#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <memory>
#include <string>

using namespace FRDDYL002;

// Helper function to create a test PGM image
void createTestPGM(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    int width = 20, height = 20;
    file << "P5\n" << width << " " << height << "\n255\n";
    
    // Create a simple image with two components
    unsigned char buffer[400]; // 20x20
    std::memset(buffer, 0, sizeof(buffer));
    
    // Component 1: 4x4 square in the corner
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            buffer[y * width + x] = 200;
        }
    }
    
    // Component 2: 8x8 square in the middle
    for (int y = 6; y < 14; y++) {
        for (int x = 6; x < 14; x++) {
            buffer[y * width + x] = 200;
        }
    }
    
    file.write(reinterpret_cast<char*>(buffer), width * height);
    file.close();
}

// Clean up test files
void cleanupTestFiles(const std::string& filename) {
    if (std::ifstream(filename).good()) {
        std::remove(filename.c_str());
    }
}

TEST_CASE("PGMimageProcessor Class Unit Tests") {
    // Create a test image for all the tests
    std::string testFile = "test_image.pgm";
    createTestPGM(testFile);
    
    SECTION("Constructor Test") {
        PGMimageProcessor processor(testFile);
        // Basic check - we can't assert much about the internals without accessors
        REQUIRE_NOTHROW(processor.getComponentCount());
    }

    SECTION("Copy Constructor Test") {
        PGMimageProcessor original(testFile);
        original.extractComponents(100, 1);
        
        // Use copy constructor
        PGMimageProcessor copy(original);
        
        // Verify the copy has the same state
        REQUIRE(copy.getComponentCount() == original.getComponentCount());
        REQUIRE(copy.getLargestSize() == original.getLargestSize());
        REQUIRE(copy.getSmallestSize() == original.getSmallestSize());
    }
    
    SECTION("Move Constructor Test") {
        PGMimageProcessor original(testFile);
        original.extractComponents(100, 1);
        int originalCount = original.getComponentCount();
        
        // Use move constructor
        PGMimageProcessor moved(std::move(original));
        
        // Verify moved object has taken ownership
        REQUIRE(moved.getComponentCount() == originalCount);
        
        // Original should be in a valid but unspecified state
        // We can't assert much about it
    }
    
    SECTION("Copy Assignment Test") {
        PGMimageProcessor original(testFile);
        original.extractComponents(100, 1);
        
        PGMimageProcessor copy;
        copy = original;
        
        // Verify the copy has the same state
        REQUIRE(copy.getComponentCount() == original.getComponentCount());
        REQUIRE(copy.getLargestSize() == original.getLargestSize());
        REQUIRE(copy.getSmallestSize() == original.getSmallestSize());
    }
    
    SECTION("Move Assignment Test") {
        PGMimageProcessor original(testFile);
        original.extractComponents(100, 1);
        int originalCount = original.getComponentCount();
        
        PGMimageProcessor moved;
        moved = std::move(original);
        
        // Verify moved object has taken ownership
        REQUIRE(moved.getComponentCount() == originalCount);
        
        // Original should be in a valid but unspecified state
        // We can't assert much about it
    }
    
    SECTION("Extract Components Test") {
        PGMimageProcessor processor(testFile);
        
        // Extract all components with threshold 100
        int count = processor.extractComponents(100, 1);
        // Should find 2 components (from our test image)
        REQUIRE(count == 2);
        REQUIRE(processor.getComponentCount() == 2);
        
        processor.loadImage(testFile);
        // Extract with minimum size 20 (should only get the large component)
        count = processor.extractComponents(100, 20);
        REQUIRE(count == 1);
        REQUIRE(processor.getComponentCount() == 1);
        
        // No components above threshold 250
        count = processor.extractComponents(250, 1);
        REQUIRE(count == 0);
        REQUIRE(processor.getComponentCount() == 0);
    }
    
    SECTION("Filter Components By Size Test") {
        PGMimageProcessor processor(testFile);
        processor.extractComponents(100, 1);  // Extract all components first
        
        // Filter to only include large components
        int count = processor.filterComponentsBySize(50, 1000);
        REQUIRE(count == 1);  // Only the 8x8 square (64 pixels)
        
        processor.loadImage(testFile);

        // Reset and filter to only include small components
        processor.extractComponents(100, 1);
        count = processor.filterComponentsBySize(1, 20);
        REQUIRE(count == 1);  // Only the 4x4 square (16 pixels)
        
        processor.loadImage(testFile);
        
        // Reset and filter to include both
        processor.extractComponents(100, 1);
        count = processor.filterComponentsBySize(1, 1000);
        REQUIRE(count == 2);  // Both components
    }
    
    SECTION("Component Count Test") {
        PGMimageProcessor processor(testFile);
        
        // Initially no components
        REQUIRE(processor.getComponentCount() == 0);
        
        // After extraction
        processor.extractComponents(100, 1);
        REQUIRE(processor.getComponentCount() == 2);
        
        // After filtering
        processor.filterComponentsBySize(50, 1000);
        REQUIRE(processor.getComponentCount() == 1);
    }
    
    SECTION("Component Size Tests") {
        PGMimageProcessor processor(testFile);
        processor.extractComponents(100, 1);
        
        // Largest component is the 8x8 square (64 pixels)
        REQUIRE(processor.getLargestSize() == 64);
        
        // Smallest component is the 4x4 square (16 pixels)
        REQUIRE(processor.getSmallestSize() == 16);
    }
    
    SECTION("Write Components Test") {
        PGMimageProcessor processor(testFile);
        processor.extractComponents(100, 1);
        
        std::string outputFile = "output_test.pgm";
        bool result = processor.writeComponents(outputFile);
        
        REQUIRE(result == true);
        // Check file exists
        REQUIRE(std::ifstream(outputFile).good() == true);
        
        // Clean up output file
        cleanupTestFiles(outputFile);
    }
    
    // Clean up the test file
    cleanupTestFiles(testFile);
}