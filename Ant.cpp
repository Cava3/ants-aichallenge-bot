#include "Ant.h"
// #include "AStar.h"
// #include "Location.h"
// #include "State.h"


// using namespace std;


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

    // Variables pour la direction et la destination
    // AStar pathfinder = AStar();
    Location nLoc;
    int direction;


    // On pathfind vers la nourriture la plus proche
    Location closestFood = findClosestFood(state);
    // _pathfinder.pathfind(state, _position, closestFood);
    // vector<Location> path = _pathfinder.getPath();

    if(_path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << std::endl;
        return -1;
    }

    nLoc = _path[0];

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

    return closestFood;
}
