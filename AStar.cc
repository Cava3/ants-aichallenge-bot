#include <vector>

#include "State.h"
#include "Node.hh"

/*
    class for finding a path between two locations on the game map.
*/
class AStar {
    std::vector<Node> _path;
    std::vector<Node> _toVisit;
    int _maxDistance;
    State* _state;

    AStar();
    ~AStar();

    // Fonctions publiques d'intéraction avec le module
    std::vector<Node> getPath();
    void pathfind(State* state, Location* start, Location* end);
    void reset();

    // Fonctions privées utilitaires
    void _addAdjacentNodes(Node* node, State* state);
    void _addNode(Node* node, State* state);
    void _sortToVisit();
    void _pathfindLoop(State* state);
    void _reset();


};
