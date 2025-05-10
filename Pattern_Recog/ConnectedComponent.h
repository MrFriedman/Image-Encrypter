// encapsulates information about a connected component

#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H

#include <vector>
#include <utility>

using namespace std;
namespace FRDDYL002 {

class ConnectedComponent {
private:
    //int componentSize;   // Number of pixel in the component
    int componentID;
    std::vector<pair<int, int>> pixels;

public:
    // Constructors & Destructor (Big 6)
    ConnectedComponent();
    ConnectedComponent(int id);
    ~ConnectedComponent();

    ConnectedComponent(const ConnectedComponent& other);
    ConnectedComponent& operator=(const ConnectedComponent& other);
    
    ConnectedComponent(ConnectedComponent&& other) noexcept;
    ConnectedComponent& operator=(ConnectedComponent&& other) noexcept;

    // Getters
    int getSize() const;
    int getID() const;
    const vector<pair<int, int>>& getPixels() const;
    // Setters
    void addPixel(int x, int y);
    void setComponentID(int componentID);

};

} // namespace FRDDYL002

#endif // CONNECTEDCOMPONENT_H
