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
    for(std::vector<Node*>::iterator it = _path.begin(); it != _path.end(); ++it) {
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

    Node* startNode = _createNode(start, end);
    startNode->distanceFromStart = 0;
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


void AStar::_pathfindLoop(const State& state, const Location& end) {
    while(!_toVisit.empty()) {
        const_cast<State&>(state).bug << "0" << endl;

        // On récupère le meilleur noeud à visiter
        Node* bestNode = _getBestFromList(state);

        const_cast<State&>(state).bug << "1" << endl;

        // On choisis le noeud à visiter et on le visite
        _visitNode(bestNode, state, end);

        const_cast<State&>(state).bug << "2" << endl;
        const_cast<State&>(state).bug << _toVisit.size() << endl;
    }
}

void AStar::_visitNode(Node* node_ptr, const State& state, const Location& end) {
    // On valide la visite du noeud
    node_ptr->explored = true;
    const_cast<State&>(state).bug << "1.0" << endl;
    _toVisit.erase(std::find(_toVisit.begin(), _toVisit.end(), node_ptr));
    _visited.push_back(node_ptr);

    const_cast<State&>(state).bug << "1.1" << endl;

    // Si la distance est trop grande, on arrête
    if(_getNodeScore(node_ptr, state) > _maxDistance) {
        return;
    }

    // Si on est arrivé à la fin, on arrête
    if(node_ptr->location == end) {
        const_cast<State&>(state).bug << "On est à la fin" << endl;
        const_cast<State&>(state).bug << &node_ptr << " | " << node_ptr->previousNode << endl;
        _maxDistance = node_ptr->distanceFromStart;
        _validatePath(node_ptr, state);
        return;
    }

    const_cast<State&>(state).bug << "1.2" << endl;
    // On ajoute les noeuds adjacents
    _addAdjacentNodes(node_ptr, state);
}

void AStar::_addAdjacentNodes(Node* node_ptr, const State& state) {
    Location location = node_ptr->location;
    Node* previousNode = node_ptr;
    const_cast<State&>(state).bug << previousNode << endl;

    for(int d=0; d<TDIRECTIONS; d++) {
        // Je créer une Location à partir de la direction
        Location adjacentLocation = state.getLocation(location, d);
        const_cast<State&>(state).bug << "1.2.0" << endl;
        // Je vois si je peux m'y déplacer
        if(_isLocationValid(adjacentLocation, state)) {
            // BUG: ICI EST LE PROBLEME
            // BUG: adjacentNode est désalloué en sortie de scope
            Node* adjacentNode = _createNode(adjacentLocation, _endLocation);
            const_cast<State&>(state).bug << &adjacentNode << endl;

            if(_getNodeScore(adjacentNode, state) > _maxDistance) {
                continue;
            }

            const_cast<State&>(state).bug << previousNode << endl;
            const_cast<State&>(state).bug << "1.2.1" << endl;

            // Je vois si je l'ai déjà visité
            std::vector<Node*>::iterator it = std::find(_visited.begin(), _visited.end(), adjacentNode);
            if(it != _visited.end()) {
                // On regarde si on a trouvé un chemin plus court
                if((*it)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    (*it)->distanceFromStart = node_ptr->distanceFromStart + 1;
                    (*it)->previousNode = previousNode;
                    _toVisit.push_back(*it);
                    _visited.erase(it);
                    const_cast<State&>(state).bug << "2.2.1" << endl;
                }
                continue;
            }

            // FIXME: Code doublé
            // FIXME: Trop d'iendentation => mise en fonction

            // Je vois si je l'ai déjà ajouté à la liste à visiter
            it = std::find(_toVisit.begin(), _toVisit.end(), adjacentNode);
            if(it != _toVisit.end()) {
                // On regarde si on a trouvé un chemin plus court
                if((*it)->distanceFromStart > node_ptr->distanceFromStart + 1) {
                    (*it)->distanceFromStart = node_ptr->distanceFromStart + 1;
                    (*it)->previousNode = previousNode;
                    // Vu que je modifie directement l'objet dans la liste, pas besoin de le réajouter
                    const_cast<State&>(state).bug << "2.2.2" << endl;
                }
                continue;
            }

            const_cast<State&>(state).bug << "1.2.3" << endl;
            adjacentNode->previousNode = previousNode;
            adjacentNode->distanceFromStart = previousNode->distanceFromStart + 1;

            _toVisit.push_back(adjacentNode);
            const_cast<State&>(state).bug << "1.2.4" << endl;
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
void AStar::_validatePath(Node* node_ptr, const State& state) {
    Node* currentNode = node_ptr;
    _path.clear();
    while(currentNode->distanceFromStart > 0) {
        _path.push_back(currentNode); // Le chemin commence par la fin, on l'inversera à la récupération.
        currentNode = currentNode->previousNode;
        const_cast<State&>(state).bug << node_ptr->location.col << " ; " << node_ptr->previousNode->location.col << endl;
        const_cast<State&>(state).bug << currentNode << endl;
    }
}

double AStar::_distanceToEnd(const Node* node_ptr, const State& state) { // Volé depuis State
    Location loc1 = node_ptr->location;
    Location loc2 = node_ptr->destination;
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, state.rows-d1),
        dc = min(d2, state.cols-d2);
    return sqrt(dr*dr + dc*dc);
};

Node* AStar::_createNode(const Location& location, const Location& end) {
    Node* node_ptr = new Node(location, end);
    _every_node_ever_created.push_back(node_ptr);
    return node_ptr;
}