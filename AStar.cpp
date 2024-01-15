#include <vector>
#include <algorithm>

#include "AStar.h"
#include "State.h"
#include "Square.h"


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
    for(std::vector<Node*>::reverse_iterator it = _path.rbegin(); it != _path.rend(); ++it) {
        path.push_back((*it)->location);
    }

    return path;
}

// Lance le pathfind dans le contexte de `state`, de `start` à `end`
void AStar::pathfind(const State& state, const Location& start, const Location& end){
    _reset();
    _endLocation = end;
    _rows = state.rows;
    _cols = state.cols;
    _maxDistance = 20 * (int) state.distance(start, end); // Arbitraire
     

    Node* startNode = _createNode(start, end);
    startNode->distanceFromStart = 0;
    _toVisit.push_back(startNode);

    if(start == end || !state.isLocationReachable(end)) {
        _validatePath(startNode);
        return;
    }

    _pathfindLoop(state, end);
}

// Remet à 0 le calculateur. Est appelé automatiquement par `pathfind`
void AStar::reset() {
    _reset();
}

//==========================================================================
//##                    Fonctions privées utilitaires                     ##
//==========================================================================


void AStar::_pathfindLoop(const State& state, const Location& end) {
    while(!_toVisit.empty() && const_cast<State&>(state).timer.getTime() < state.turntime - 20) {
        // const_cast<State&>(state).bug << "On loop" << std::endl;

        // On récupère le meilleur noeud à visiter
        Node* bestNode = _getBestFromList(state);

        // On choisis le noeud à visiter et on le visite
        _visitNode(bestNode, state, end);

        // const_cast<State&>(state).bug << "to visit : " << _toVisit.size() << std::endl;
    }
}

void AStar::_visitNode(Node* node_ptr, const State& state, const Location& end) {
    // On valide la visite du noeud
    node_ptr->explored = true;
    _toVisit.erase(std::find(_toVisit.begin(), _toVisit.end(), node_ptr));
    _visited.push_back(node_ptr);

    // Si la distance est trop grande, on arrête
    if(_getNodeScore(node_ptr, state) > _maxDistance) {
        return;
    }

    // Si on est arrivé à la fin, on stock le chemin
    if(node_ptr->location == end) {
        // const_cast<State&>(state).bug << "On est à la fin" << std::endl;
        _maxDistance = node_ptr->distanceFromStart;
        _validatePath(node_ptr);
        return;
    }

    // On ajoute les noeuds adjacents
    _addAdjacentNodes(node_ptr, state);
}

void AStar::_addAdjacentNodes(Node* node_ptr, const State& state) {
    Location location = node_ptr->location;
    Node* previousNode = node_ptr;

    // const_cast<State&>(state).bug << "From : " << previousNode->location.col << "," << previousNode->location.row << std::endl;
    for(int d=0; d<TDIRECTIONS; d++) {
        // Je créer une Location à partir de la direction
        Location adjacentLocation = state.getLocation(location, d);
        // Je vois si je peux m'y déplacer
        if(state.isLocationReachable(adjacentLocation)) {
            Node* adjacentNode_ptr = _createNode(adjacentLocation, _endLocation);

            if(_getNodeScore(adjacentNode_ptr, state) >= _maxDistance) {
                continue;
            }


            // Je vois si je l'ai déjà visité
            std::vector<Node*>::iterator it;
            bool trouve = false;
            for(it = _visited.begin(); it != _visited.end(); ++it) {
                if((*it)->location == adjacentLocation) {
                    trouve = true;
                    break;
                }
            }

            if(trouve) {
                // On regarde si on a trouvé un chemin plus court
                if((*it)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    _visited.erase(it);
                } else {
                    continue;
                }
            }

            // Je vois si je l'ai déjà ajouté à la liste à visiter
            trouve = false;
            for(it = _toVisit.begin(); it != _toVisit.end(); ++it) {
                if((*it)->location == adjacentLocation) {
                    trouve = true;
                    break;
                }
            }

            if(trouve) {
                // On regarde si on a trouvé un chemin plus court
                if((*it)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    _toVisit.erase(it);
                } else {
                    continue;
                }
            }

            adjacentNode_ptr->previousNode = previousNode;
            adjacentNode_ptr->distanceFromStart = previousNode->distanceFromStart + 1;

            _toVisit.push_back(adjacentNode_ptr);
        }
    }
}

// Retourne le meilleur noeud à visiter
Node* AStar::_getBestFromList(const State& state) {
    Node* bestNode = _toVisit[0];
    for(std::vector<Node*>::iterator it = _toVisit.begin(); it != _toVisit.end(); ++it) {
        if(_getNodeScore(*it, state) < _getNodeScore(bestNode, state)) {
            bestNode = *it;
        }
    }
    return bestNode;
}

// Retourne le score d'un noeud
double AStar::_getNodeScore(const Node* node_ptr, const State& state) {
    return node_ptr->distanceFromStart + _distanceToEnd(node_ptr, state);
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

// Fonction qui génère le chemin à partir du noeud final
void AStar::_validatePath(Node* node_ptr) {
    Node* currentNode = node_ptr;
    _path.clear();
    while(currentNode->distanceFromStart > 0) {
        _path.push_back(currentNode); // Le chemin commence par la fin, on l'inversera à la récupération.
        currentNode = currentNode->previousNode;
    }
}

double AStar::_distanceToEnd(const Node* node_ptr, const State& state) { // Volé depuis State
    Location loc1 = node_ptr->location;
    Location loc2 = node_ptr->destination;
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = std::min(d1, state.rows-d1),
        dc = std::min(d2, state.cols-d2);
    return sqrt(dr*dr + dc*dc);
};

Node* AStar::_createNode(const Location& location, const Location& end) {
    Node* node_ptr = new Node(location, end);
    return node_ptr;
}