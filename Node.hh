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
    Location destination;

    Node(Location location)
    {
        explored = false;
        distanceFromStart = 0;
        previousNode = NULL;
        this->location = location;
    };

    Node(Location location, Location destination)
    {
        explored = false;
        distanceFromStart = 0;
        previousNode = NULL;
        this->location = location;
        this->destination = destination;
    };

    void reset()
    {
        distanceFromStart = 0;
    };

    bool operator==(const Node& other) const
    {
        return location == other.location && destination == other.destination;
    }
};

#endif //NODE_HH_
