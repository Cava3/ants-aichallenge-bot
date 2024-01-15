#pragma once

#include "AStar.h"
#include "Location.h"

class State;


class Ant
{
    private:
        Location _position;
        Location _nextTurnPosition;
        Location _destination;
        AStar _pathfinder;
        std::vector<Location> _path;
        int _selectDirection(const State& state, double timeLimit);
        void _setDestination(State& state, Location location);
        Location _findClosestFood(const State& state);
        void _makeMove(State& state, int direction);

    public:
        Ant(int id, State& state, Location location);
        int id;
        Location getPosition();
        Location getNextTurnPosition();
        Location getDestination();
        void playTurn(State& state, double timeLimit);
        Location takeDecision(const State& state, double timeLimit);
        void validateLastTurnMove(State& state, bool validated);
};


// TO DO :
// - passer le state en variable globale de la classe
