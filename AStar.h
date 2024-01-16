#pragma once

#include <vector>

#include "Location.h"
#include "Node.h"

class State;


/*
    class for finding a path between two locations on the game map.
*/
class AStar {
    private :
        std::vector<Node*> _toVisit;
        std::vector<Node*> _visited;
        std::vector<Node*> _path;
        
        Location _endLocation;
        int _maxDistance;
        int _rows, _cols;

        void _visitNode(Node* node, const State& state, const Location& end);
        Node* _createNode(Location& location, Location& end);
        double _distanceToEnd(Node* node1, const State& state);
        void _pathfindLoop(const State& state, const Location& end);
        double _getNodeScore(Node* node, const State& state);
        void _addAdjacentNodes(Node* node, const State& state);
        void _validatePath(Node* node);
        Node* _getBestFromList(const State& state);
        void _reset();

    public :
        AStar();

        std::vector<Location*>* getPath(const State& state);
        void pathfind(const State& state, Location& start, Location& end);
        void reset();
};
