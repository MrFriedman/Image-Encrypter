#include "ConnectedComponent.h"

using namespace std;
namespace FRDDYL002 {

// Constructor
ConnectedComponent::ConnectedComponent(int id) : componentID(id) {}

// Destructor
ConnectedComponent::~ConnectedComponent() = default;

// Copy Constructor
ConnectedComponent::ConnectedComponent(const ConnectedComponent& other)
    : componentID(other.componentID), pixels(other.pixels) {}

// Copy Assignment
ConnectedComponent& ConnectedComponent::operator=(const ConnectedComponent& other) {
    if (this != &other) {
        componentID = other.componentID;
        pixels = other.pixels;
    }
    return *this;
}

// Move Constructor
ConnectedComponent::ConnectedComponent(ConnectedComponent&& other) noexcept
    : componentID(other.componentID), pixels(std::move(other.pixels)) {}

// Move Assignment
ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent&& other) noexcept {
    if (this != &other) {
        componentID = other.componentID;
        pixels = std::move(other.pixels);
    }
    return *this;
}

// Get component size
int ConnectedComponent::getSize() const {
    return pixels.size();
}

// Get component ID
int ConnectedComponent::getID() const {
    return componentID;
}

// Get pixels
const vector<pair<int, int>>& ConnectedComponent::getPixels() const {
    return pixels;
}

// Add a pixel to the component
void ConnectedComponent::addPixel(int x, int y) {
    pixels.push_back(make_pair(x, y));
    //componentSize++;
}

} // namespace FRDDYL002
