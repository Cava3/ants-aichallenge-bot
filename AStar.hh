#ifndef ASTAR_HH_
#define ASTAR_HH_

#include <vector>

#include "State.h"
#include "Node.hh"

/*
    class for finding a path between two locations on the game map.
*/
class AStar {
private :
    std::vector<Node> _path;
    std::vector<Node> _toVisit;
    int _maxDistance;
    State* _state;

    void _addAdjacentNodes(Node* node, State* state);
    void _addNode(Node* node, State* state);
    void _sortToVisit();
    void _pathfindLoop(State* state);
    void _reset();

public :
    AStar();
    ~AStar();

    std::vector<Node> getPath();
    void pathfind(State* state, Location* start, Location* end);
    void reset();
};


#endif // ASTAR_HH_
