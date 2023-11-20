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
        std::vector<Node> _toVisit;
        std::vector<Node> _visited;
        std::vector<Node> _path;
        Location _endLocation;
        int _maxDistance;
        int _rows, _cols;

        bool _isLocationValid(const Location& location, const State& state);
        void _addAdjacentNodes(Node& node, const State& state);
        void _visitNode(Node& node, const State& state);
        static double _distanceToEnd(const Node& node);
        void _pathfindLoop(const State& state);
        void _validatePath(Node& node);
        void _reset();

    public :
        AStar();
        ~AStar();

        std::vector<Location> getPath();
        void pathfind(State state, const Location& start, const Location& end) {}
        void reset();
};


#endif // ASTAR_HH_
