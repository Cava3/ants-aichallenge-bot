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
    std::vector<Node> _visited;
    int _maxDistance;

    AStar() {
        _reset();
    }
    ~AStar(){}

    //==========================================================================
    //##           Fonctions publiques d'intéraction avec le module           ##
    //==========================================================================

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

    //==========================================================================
    //##                    Fonctions privées utilitaires                     ##
    //==========================================================================

    void _addAdjacentNodes(Node& node, const State& state) {
        Location location = node.location;
        Node* previousNode = &node;

        for(int d=0; d<TDIRECTIONS; d++) {
            // Je créer une Location à partir de la direction
            Location adjacentLocation = state.getLocation(location, d);

            // Je vois si je peux m'y déplacer
            if(_isLocationValid(adjacentLocation, state)) {
                Node adjacentNode = Node(adjacentLocation);

                // Je vois si je l'ai déjà visité
                std::vector<Node>::iterator it = std::find(_visited.begin(), _visited.end(), adjacentNode);
                if(it != _visited.end()) {
                    // On regarde si on a trouvé un chemin plus court
                    if(it->distanceFromStart > node.distanceFromStart + 1) {
                        it->distanceFromStart = node.distanceFromStart + 1;
                        it->previousNode = previousNode;
                        _toVisit.push_back(*it);
                        _visited.erase(it);
                    }
                    continue;
                }
                // FIXME: Code doublé
                // FIXME: Trop d'iendentation, mise en fonction

                // Je vois si je l'ai déjà ajouté à la liste à visiter
                it = std::find(_toVisit.begin(), _toVisit.end(), adjacentNode);
                if(it != _toVisit.end()) {
                    // On regarde si on a trouvé un chemin plus court
                    if(it->distanceFromStart > node.distanceFromStart + 1) {
                        it->distanceFromStart = node.distanceFromStart + 1;
                        it->previousNode = previousNode;
                        // Vu que je modifie directement l'objet dans la liste, pas besoin de le réajouter
                    }
                    continue;
                }

                adjacentNode.previousNode = previousNode;
                adjacentNode.distanceFromStart = node.distanceFromStart + 1;
                _toVisit.push_back(adjacentNode);
            }
        }
    }


    void _visitNode(Node& node, const State& state, const Location& end) {
        // On valide la visite du noeud
        node.explored = true;
        _toVisit.pop_back();
        _visited.push_back(node);

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

        // On ajoute les noeuds adjacents
        _addAdjacentNodes(node, state);
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

    bool _isLocationValid(const Location& location, const State& state){
        return true;
    }

    void _validatePath(Node& node) {
        _path.clear();
        while(node.previousNode != NULL) {
            _path.push_back(node);
            node = *node.previousNode;
        }
    }

};
