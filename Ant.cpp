#include "Ant.h"
#include "State.h"


Ant::Ant(int id, State& state, Location location) {
    this -> id = id;
    _position = location;
    _destination = _position;
    _pathfinder = AStar();
    const_cast<State&>(state).bug << "New ant #" << id << ". Position : " << _position.row << ":" << _position.col << std::endl;
}

Location Ant::getPosition() {
    return _position;
}

Location Ant::getNextTurnPosition() {
    return _nextTurnPosition;
}

Location Ant::getDestination() {
    return _destination;
}

void Ant::playTurn(State& state, double timeLimit) {
    const_cast<State&>(state).bug << "Ant::playTurn()" << std::endl;
    const_cast<State&>(state).bug << "Ant " << id << " is playing" << std::endl;
    const_cast<State&>(state).bug << "Location " << _position.row << ":" << _position.col << std::endl;

    Location closestFood = _findClosestFood(state);

    // Sale, trouver un moyen de comparer les 2 Locations + proprement
    if(closestFood.row != _destination.row || closestFood.col != _destination.col) {
        _setDestination(state, closestFood);
    }

    int direction = _selectDirection(state, timeLimit);

    if(direction == -1) {
        const_cast<State&>(state).bug << "Ant " << id << " goes nowhere" << std::endl;
        return;
    }

    _makeMove(state, direction);
}

void Ant::_setDestination(State& state, Location location) {
    const_cast<State&>(state).bug << "Ant::_setDestination()" << std::endl;
    _destination = location;
    _pathfinder.pathfind(state, _position, _destination);
    _path = _pathfinder.getPath();

    if(_path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << std::endl;
    }
}

int Ant::_selectDirection(const State& state, double timeLimit) {
    const_cast<State&>(state).bug << "Ant::_selectDirection()" << std::endl;

    // Variables pour la direction et la destination
    Location nLoc;
    int direction;
    nLoc = _path[0];

    // On détermine la direction en fonction de la position
    for(direction=0; direction<TDIRECTIONS; direction++) {
        Location lookingLocation = state.getLocation(_position, direction);
        // const_cast<State&>(state).bug << "lookingLocation: " << lookingLocation.col << "," << lookingLocation.row << std::endl;
        // const_cast<State&>(state).bug << "nLoc: " << nLoc.col << "," << nLoc.row << std::endl;
        if(lookingLocation == nLoc) {
            break;
        }
    }

    const_cast<State&>(state).bug << "Ant " << id << " goes " << direction << std::endl;

    return direction;
}

Location Ant::_findClosestFood(const State& state){
    // TODO : calculer la distance en fonction du AStar, pas à vol d'oiseau
    const_cast<State&>(state).bug << "Ant::_findClosestFood()" << std::endl;

    Location closestFood = state.food[0];

    for(int i = 1; i < state.food.size(); i++){
        int antPosCol = _position.col;
        int antPosRow = _position.row;
        int currentFoodDistance = (
            abs(closestFood.col - antPosCol) +
            abs(closestFood.row - antPosRow)
            );
        int newFoodDistance = (
            abs(state.food[i].col - antPosCol) +
            abs(state.food[i].row - antPosRow)
            );
        
        if (newFoodDistance < currentFoodDistance){
            closestFood = state.food[i];
        }
    }

    const_cast<State&>(state).bug << "Closest food is " << closestFood.row << ":" << closestFood.col << std::endl;

    return closestFood;
}

void Ant::_makeMove(State& state, int direction) {
    const_cast<State&>(state).bug << "Ant::_makeMove()" << std::endl;
    const_cast<State&>(state).bug << "Current location " << _position.row << ":" << _position.col << std::endl;

    Location destination = state.getLocation(_position, direction);
    const_cast<State&>(state).bug << "Destination " << destination.row << ":" << destination.col << std::endl;

    // Si on se dirige vers une case occupée par une fourmi à nous
    if(state.isAntPosition(destination)) {
        return;
    }

    _nextTurnPosition = state.getLocation(_position, direction);
    state.makeMove(_position, direction);
}

void Ant::validateLastTurnMove(State& state, bool validated) {
    if(validated) {
        _position = _nextTurnPosition;
        _path.erase(_path.begin());
    }
    else {
        _nextTurnPosition = _position;
        _setDestination(state, _destination);
    }
}