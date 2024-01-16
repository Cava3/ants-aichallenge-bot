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
    for(std::vector<Node*>::reverse_iterator iterator = _path.rbegin(); iterator != _path.rend(); ++iterator) {
        path.push_back((*iterator)->location);
    }

    return path;
}

// Lance le pathfind dans le contexte de `state_ref`, de `start_ref` à `end`
void AStar::pathfind(const State& state_ref, const Location& start_ref, const Location& end_ref){
    _reset();
    _endLocation = end_ref;
    _rows = state_ref.rows;
    _cols = state_ref.cols;
    _maxDistance = 20 * (int) state_ref.distance(start_ref, end_ref); // Arbitraire
     

    Node* startNode_ptr = _createNode_ptr(start_ref, end_ref);
    startNode_ptr->distanceFromStart = 0;
    _toVisit.push_back(startNode_ptr);

    if(start_ref == end_ref || !state_ref.isLocationReachable(end_ref)) {
        _validatePath(startNode_ptr);
        return;
    }

    _pathfindLoop(state_ref, end_ref);
}

// Remet à 0 le calculateur. Est appelé automatiquement par `pathfind`
void AStar::reset() {
    _reset();
}

//==========================================================================
//##                    Fonctions privées utilitaires                     ##
//==========================================================================


void AStar::_pathfindLoop(const State& state_ref, const Location& end_ref) {
    while(!_toVisit.empty() && const_cast<State&>(state_ref).timer.getTime() < state_ref.turntime - 20) {
        // On récupère le meilleur noeud à visiter
        Node* bestNode_ref = _getBestFromList_ptr(state_ref);

        // On choisis le noeud à visiter et on le visite
        _visitNode(bestNode_ref, state_ref, end_ref);
    }
}

void AStar::_visitNode(Node* node_ptr, const State& state_ref, const Location& end_ref) {
    // On valide la visite du noeud
    node_ptr->explored = true;
    _toVisit.erase(std::find(_toVisit.begin(), _toVisit.end(), node_ptr));
    _visited.push_back(node_ptr);

    // Si la distance est trop grande, on arrête
    if(_getNodeScore(node_ptr, state_ref) > _maxDistance) {
        return;
    }

    // Si on est arrivé à la fin, on stock le chemin
    if(node_ptr->location == end_ref) {
        _maxDistance = node_ptr->distanceFromStart;
        _validatePath(node_ptr);
        return;
    }

    // On ajoute les noeuds adjacents
    _addAdjacentNodes(node_ptr, state_ref);
}

void AStar::_addAdjacentNodes(Node* node_ptr, const State& state_ref) {
    Location location = node_ptr->location;
    Node* previousNode_ptr = node_ptr;

    for(int d=0; d<TDIRECTIONS; d++) {
        // On crée une Location à partir de la direction
        Location adjacentLocation = state_ref.getLocation(location, d);
        // On voit si je peux m'y déplacer
        if(state_ref.isLocationReachable(adjacentLocation)) {
            Node* adjacentNode_ptr = _createNode_ptr(adjacentLocation, _endLocation);

            if(_getNodeScore(adjacentNode_ptr, state_ref) >= _maxDistance) {
                continue;
            }


            // On voit si je l'ai déjà visité
            std::vector<Node*>::iterator iterator;
            bool trouve = false;
            for(iterator = _visited.begin(); iterator != _visited.end(); ++iterator) {
                if((*iterator)->location == adjacentLocation) {
                    trouve = true;
                    break;
                }
            }

            if(trouve) {
                // On regarde si on a trouvé un chemin plus court
                if((*iterator)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    _visited.erase(iterator);
                } else {
                    continue;
                }
            }

            // On voit si je l'ai déjà ajouté à la liste à visiter
            trouve = false;
            for(iterator = _toVisit.begin(); iterator != _toVisit.end(); ++iterator) {
                if((*iterator)->location == adjacentLocation) {
                    trouve = true;
                    break;
                }
            }

            if(trouve) {
                // On regarde si on a trouvé un chemin plus court
                if((*iterator)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    _toVisit.erase(iterator);
                } else {
                    continue;
                }
            }

            adjacentNode_ptr->previousNode_ptr = previousNode_ptr;
            adjacentNode_ptr->distanceFromStart = previousNode_ptr->distanceFromStart + 1;

            _toVisit.push_back(adjacentNode_ptr);
        }
    }
}

// Retourne le meilleur noeud à visiter
Node* AStar::_getBestFromList_ptr(const State& state_ref) {
    Node* bestNode_ref = _toVisit[0];
    for(std::vector<Node*>::iterator iterator = _toVisit.begin(); iterator != _toVisit.end(); ++iterator) {
        if(_getNodeScore(*iterator, state_ref) < _getNodeScore(bestNode_ref, state_ref)) {
            bestNode_ref = *iterator;
        }
    }
    return bestNode_ref;
}

// Retourne le score d'un noeud
double AStar::_getNodeScore(const Node* node_ptr, const State& state_ref) {
    return node_ptr->distanceFromStart + _distanceToEnd(node_ptr, state_ref);
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
        currentNode = currentNode->previousNode_ptr;
    }
}

double AStar::_distanceToEnd(const Node* node_ptr, const State& state_ref) { // Volé depuis State
    Location loc1 = node_ptr->location;
    Location loc2 = node_ptr->destination;
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = std::min(d1, state_ref.rows-d1),
        dc = std::min(d2, state_ref.cols-d2);
    return sqrt(dr*dr + dc*dc);
};

Node* AStar::_createNode_ptr(const Location& location, const Location& end_ref) {
    Node* node_ptr = new Node(location, end_ref);
    return node_ptr;
}