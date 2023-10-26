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

    AStar() {
        _reset();
    }
    ~AStar(){}

    // Fonctions publiques d'intéraction avec le module
    std::vector<Location> getPath() {
        std::vector<Location> path;
        for(int i=0; i<(int)_path.size(); i++) {
            path.push_back(_path[i].location);
        }
        return path;
    }

    void pathfind(const State& state, const Location& start, const Location& end){
        _reset();

        Node startNode = Node(start);
        startNode.distanceFromStart = 0;
        _toVisit.push_back(startNode);
        _pathfindLoop(state);
    }
    
    void reset() {
        _reset();
    }

    // Fonctions privées utilitaires
    void _addAdjacentNodes(Node& node, const State& state);
    void _visitNode(Node& node, const State& state) {}
    void _sortToVisit(const State& state);
    void _pathfindLoop(const State& state);

    void _reset(){
        _path.clear();
        _toVisit.clear();
        _maxDistance = 9999999;
    }


};
