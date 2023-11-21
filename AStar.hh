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

        void _visitNode(Node& node, const State& state, const Location& end);
        bool _isLocationValid(const Location& location, const State& state);
        double _distanceToEnd(const Node& node1, const State& state);
        void _pathfindLoop(const State& state, const Location& end);
        double _getNodeScore(const Node& node, const State& state);
        void _addAdjacentNodes(Node& node, const State& state);
        Node _getBestFromList(const State& state);
        void _validatePath(Node& node);
        void _reset();

    public :
        AStar();

        std::vector<Location> getPath();
        void pathfind(const State& state, const Location& start, const Location& end) {}
        void reset();
};


#endif // ASTAR_HH_
