#ifndef NODE_HH_
#define NODE_HH_

#include "State.h"

/*
    struct for representing a node in the AStar path.
*/
struct Node
{
    int distanceFromStart;
    bool explored;
    Location location;
    Node* previousNode;

    Node(Location location)
    {
        explored = false;
        distanceFromStart = 9999999;
        previousNode = NULL;
        this->location = location;
    };

    void reset()
    {
        distanceFromStart = 9999999;
    };

    bool operator==(const Node& other) const
    {
        return location == other.location;
    }
};

#endif //NODE_HH_
