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
        State _state;

        void _addAdjacentNodes(Node* node, const State* state);
        void _addNode(Node* node, const State* state);
        void _sortToVisit();
        void _pathfindLoop(const State* state);
        void _reset();

    public :
        AStar();
        ~AStar();

        std::vector<Node> getPath() const;
        void pathfind(const State* state, const Location* start, const Location* end);
        void reset();
};


#endif // ASTAR_HH_
