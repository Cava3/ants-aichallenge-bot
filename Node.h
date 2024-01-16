#pragma once

#include <stdio.h>

#include "Location.h"


/*
    struct for representing a node in the AStar path.
*/
struct Node
{
    int distanceFromStart;
    bool explored;
    Location location;
    Node* previousNode_ptr;
    Location destination;

    // Constructeur sans destination
    Node(Location location)
    {
        explored = false;
        distanceFromStart = 0;
        previousNode_ptr = NULL;
        this->location = location;
    };

    // Constructeur avec destination
    Node(Location location, Location destination)
    {
        explored = false;
        distanceFromStart = 0;
        previousNode_ptr = NULL;
        this->location = location;
        this->destination = destination;
    };

    // Fonction de remise à 0 de la distance
    void reset()
    {
        distanceFromStart = 0;
    };

    // Opérateur de comparaison == (réfère à Location::operator==)
    bool operator==(const Node& other) const
    {
        return location == other.location && destination == other.destination;
    }
};
