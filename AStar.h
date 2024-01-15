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

        void _visitNode(Node* node, State& state, const Location& end);
        Node* _createNode(const Location& location, const Location& end);
        double _distanceToEnd(const Node* node1, const State& state);
        void _pathfindLoop(State& state, const Location& end);
        double _getNodeScore(const Node* node, const State& state);
        void _addAdjacentNodes(Node* node, State& state);
        void _validatePath(Node* node);
        Node* _getBestFromList(const State& state);
        void _reset();

    public :
        AStar();

        std::vector<Location> getPath();
        void pathfind(State& state, const Location& start, const Location& end);
        void reset();
};
