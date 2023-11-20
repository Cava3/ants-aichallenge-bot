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
        std::vector<Node> _path;
        int _maxDistance;

        bool _isLocationValid(const Location& location, const State& state);
        void _addAdjacentNodes(Node& node, const State& state);
        static bool _sortcompare(const Node& a, const Node& b);
        void _visitNode(Node& node, const State& state);
        static double _distanceToEnd(const Node& node);
        void _pathfindLoop(const State& state);
        void _sortToVisit(const State& state);
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
