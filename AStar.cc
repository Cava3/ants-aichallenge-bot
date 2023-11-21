#include <vector>
#include <algorithm>

#include "State.h"
#include "Node.hh"
#include "AStar.hh"

using namespace std;
/*
    class for finding a path between two locations on the game map.
*/

AStar::AStar() {
    _reset();
}

//==========================================================================
//##           Fonctions publiques d'intéraction avec le module           ##
//==========================================================================

// Retourne le chemin calculé
std::vector<Location> AStar::getPath() {
    std::vector<Location> path;
    // Penser à retourner le chemin dans le bon sens
    for(std::vector<Node>::iterator it = _path.begin(); it != _path.end(); ++it) {
        path.push_back(it->location);
    }
    return path;
}

// Lance le pathfind dans le contexte de `state`, de `start` à `end`
void AStar::pathfind(const State& state, const Location& start, const Location& end){
    _reset();
    _endLocation = end;
    _rows = state.rows;
    _cols = state.cols;

    Node startNode = Node(start, end);
    startNode.distanceFromStart = 0;
    _toVisit.push_back(startNode);

    _pathfindLoop(state, end);
}

// Remet à 0 le calculateur. Est appelé automatiquement par `pathfind`
void AStar::reset() {
    _reset();
}

//==========================================================================
//##                    Fonctions privées utilitaires                     ##
//==========================================================================

void AStar::_addAdjacentNodes(Node& node, const State& state) {
    Location location = node.location;
    Node* previousNode = &node;

    for(int d=0; d<TDIRECTIONS; d++) {
        // Je créer une Location à partir de la direction
        Location adjacentLocation = state.getLocation(location, d);

        // Je vois si je peux m'y déplacer
        if(_isLocationValid(adjacentLocation, state)) {
            Node adjacentNode = Node(adjacentLocation, _endLocation);

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
            // FIXME: Trop d'iendentation => mise en fonction

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


void AStar::_visitNode(Node& node, const State& state, const Location& end) {
    // On valide la visite du noeud
    node.explored = true;
    _toVisit.pop_back();
    _visited.push_back(node);

    // Si la distance est trop grande, on arrête
    if(_getNodeScore(node, state) > _maxDistance) {
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

void AStar::_pathfindLoop(const State& state, const Location& end) {
    while(!_toVisit.empty()) {
        // On récupère le meilleur noeud à visiter
        Node bestNode = _getBestFromList(state);

        // On supprime le noeud de la liste
        _toVisit.erase(std::find(_toVisit.begin(), _toVisit.end(), bestNode));
        
        // On choisis le noeud à visiter et on le visite
        _visitNode(bestNode, state, end);
    }
}

// Retourne le meilleur noeud à visiter
Node AStar::_getBestFromList(const State& state) {
    Node bestNode = _toVisit[0];
    for(std::vector<Node>::iterator it = _toVisit.begin(); it != _toVisit.end(); ++it) {
        if(_getNodeScore(*it, state) < _getNodeScore(bestNode, state)) {
            bestNode = *it;
        }
    }
    return bestNode;
}

// Retourne le score d'un noeud
double AStar::_getNodeScore(const Node& node, const State& state) {
    return node.distanceFromStart + _distanceToEnd(node, state);
}

// Fonction de reset entre deux pathfind
void AStar::_reset(){
    _path.clear();
    _toVisit.clear();
    _visited.clear();
    _maxDistance = 9999999;
    _endLocation = Location();
    _rows = 0;
    _cols = 0;
}

bool AStar::_isLocationValid(const Location& location, const State& state){
    // LA Location doit être dans la grille
    if(location.row < 0 || location.row >= state.rows || location.col < 0 || location.col >= state.cols)
        return false;
    
    // La Location ne doit pas être de l'eau
    if(state.grid[location.row][location.col].isWater)
        return false;

    return true;
}

// Fonction qui génère le chemin à partir du noeud final
void AStar::_validatePath(Node& node) {
    _path.clear();
    while(node.previousNode != NULL) {
        _path.push_back(node); // Le chemin commence par la fin, on l'inverse à la récupération.
        node = *node.previousNode;
    }
}

double AStar::_distanceToEnd(const Node& node1, const State& state) { // Volé depuis State
    Location loc1 = node1.location;
    Location loc2 = node1.destination;
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, state.rows-d1),
        dc = min(d2, state.cols-d2);
    return sqrt(dr*dr + dc*dc);
};
