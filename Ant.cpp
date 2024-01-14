#include "Ant.h"
#include "State.h"


Ant::Ant(int id, Location location) {
    this -> id = id;
    _position = location;
    _pathfinder = AStar();
}

Location Ant::getPosition() {
    return _position;
}

Location Ant::getTarget() {
    return _target;
}

void Ant::playTurn(State& state, double timeLimit) {
    const_cast<State&>(state).bug << "ant " << id << " is playing" << std::endl;
    int direction = selectDirection(state, timeLimit);

    if(direction == -1) {
        const_cast<State&>(state).bug << "ant " << id << " goes nowhere" << std::endl;
        return;
    }

    const_cast<State&>(state).bug << "ant " << id << " goes " << direction << std::endl;
    state.makeMove(_position, direction);
}

void Ant::setTarget(const State& state, Location location) {
    _target = location;
    _pathfinder.pathfind(state, _position, _target);
    _path = _pathfinder.getPath();
}

int Ant::selectDirection(const State& state, double timeLimit) {
    Location nLoc;
    int direction;


    // On choisis quelle action faire
    nLoc = takeDecision(state, timeLimit);
    

    if(_path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << std::endl;
        return -1;
    }

    // On détermine la direction en fonction de la position
    for(direction=0; direction<TDIRECTIONS; direction++) {
        Location lookingLocation = state.getLocation(_position, direction);
        const_cast<State&>(state).bug << "lookingLocation: " << lookingLocation.col << "," << lookingLocation.row << std::endl;
        const_cast<State&>(state).bug << "nLoc: " << nLoc.col << "," << nLoc.row << std::endl;
        if(lookingLocation == nLoc) {
            break;
        }
    }

    const_cast<State&>(state).bug << "ant " << id << " goes " << direction << std::endl;

    return direction;
}

Location Ant::findClosestFood(const State& state){
    // TODO : calculer la distance en fonction du AStar, pas à vol d'oiseau
    // Si pas de nourriture, on renvoie -1; -1
    Location closestFood = Location(-1, -1);

    for(int i = 0; i < state.food.size(); i++){
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

    return closestFood;
}


Location Ant::takeDecision(const State& state, double timeLimit) {
    
}