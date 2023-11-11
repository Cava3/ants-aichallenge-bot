#include <vector>
#include <algorithm>

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

        _pathfindLoop(state, end);
    }
    
    void reset() {
        _reset();
    }

    // Fonctions privées utilitaires
    void _addAdjacentNodes(Node& node, const State& state) {}
    void _visitNode(Node& node, const State& state, const Location& end) {
        // On valide la visite du noeud
        node.explored = true;
        _toVisit.pop_back();

        // Si la distance est trop grande, on arrête
        if(node.distanceFromStart > _maxDistance) {
            return;
        }

        // Si on est arrivé à la fin, on arrête
        if(node.location == end) {
            _maxDistance = node.distanceFromStart;
            _validatePath(node);
            return;
        }
    }

    void _sortToVisit(const State& state) {
        std::sort(_toVisit.begin(), _toVisit.end(), _sortcompare); // include <algorithm>
    }

    void _pathfindLoop(const State& state, const Location& end) {
        while(!_toVisit.empty()) {
            // On trie
            _sortToVisit(state);

            // On choisis le noeud à visiter et on le visite
            Node node = _toVisit.back();
            _visitNode(node, state, end);
        }
    }

    void _reset(){
        _path.clear();
        _toVisit.clear();
        _maxDistance = 9999999;
    }

    bool _sortcompare(const Node& a, const Node& b) {
        // TODO: Distance de l'objectif !
        return a.distanceFromStart < b.distanceFromStart;
    }

    void _validatePath(Node& node) {
        _path.clear();
        while(node.previousNode != NULL) {
            _path.push_back(node);
            node = *node.previousNode;
        }
    }

};
