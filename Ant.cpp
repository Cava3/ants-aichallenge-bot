#include "Ant.h"
#include "State.h"

Ant::Ant(int id, State& state, Location location) {
    this -> id = id;
    _position = location;
    _destination = _position;
    _pathfinder = AStar();
    const_cast<State&>(state).bug << "New ant #" << id << ". Position : " << _position.row << ":" << _position.col << std::endl;
}

Location Ant::getPosition()
{
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

    Location nextLocation = takeDecision(state, timeLimit);
    const_cast<State&>(state).bug << "Next location ## : " << nextLocation.row << ":" << nextLocation.col << std::endl;

    if(nextLocation == Location(-1, -1)) {
        const_cast<State&>(state).bug << "Ant " << id << " has no decision" << std::endl;
        return;
    }

    int direction = _selectDirection(state, nextLocation, timeLimit);

    if(direction == -1) {
        const_cast<State&>(state).bug << "Ant " << id << " goes nowhere" << std::endl;
        return;
    }

    _makeMove(state, direction);
}

void Ant::_setDestination(const State& state, Location location) {
    const_cast<State&>(state).bug << "Ant::_setDestination()" << std::endl;
    const_cast<State&>(state).bug << "From : " << _position.col << "," << _position.row << std::endl;
    const_cast<State&>(state).bug << "To : " << _destination.col << "," << _destination.row << std::endl;

    _destination = location;
    _pathfinder.pathfind(state, _position, _destination);
    const_cast<State&>(state).bug << "Ant::_setDestination() : Calculated" << std::endl;
    _path = _pathfinder.getPath(state);
    const_cast<State&>(state).bug << "Ant::_setDestination() : Path size : " << _path.size() << std::endl;

    if(_path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << std::endl;
    }

    const_cast<State&>(state).bug << "Exit Ant::_setDestination()" << std::endl;
}

int Ant::_selectDirection(const State &state, Location nLoc, double timeLimit)
{
    const_cast<State&>(state).bug << "Ant::_selectDirection()" << std::endl;

    int direction = -1;

    // Mettre ailleurs
    if (_path.size() == 0)
    {
        const_cast<State &>(state).bug << "/!\\ Empty path" << std::endl;
        return -1;
    }

    // On détermine la direction en fonction de la position
    for (int i = 0; i < TDIRECTIONS; i++)
    {
        Location lookingLocation = state.getLocation(_position, i);

        if(lookingLocation.row == nLoc.row && lookingLocation.col == nLoc.col) {
            direction = i;
            break;
        }
    }

    if(direction == -1) {
        const_cast<State&>(state).bug << "/!\\ No direction found" << std::endl;
        return -1;
    }

    const_cast<State&>(state).bug << "Ant " << id << " goes " << direction << std::endl;
    const_cast<State&>(state).bug << "Exit Ant::_selectDirection()" << std::endl;

    return direction;
}

Location Ant::_findClosestFood(const State &state)
{
    const_cast<State&>(state).bug << "Ant::_findClosestFood()" << std::endl;
    // Calculer la distance en fonction du AStar, pas à vol d'oiseau
    // Si pas de nourriture, on renvoie -1; -1
    Location closestFood = Location(-1, -1);


    for (int i = 0; i < state.food.size(); i++)
    {
        int antPosCol = _position.col;
        int antPosRow = _position.row;
        int currentFoodDistance = (abs(closestFood.col - antPosCol) +
                                   abs(closestFood.row - antPosRow));
        int newFoodDistance = (abs(state.food[i].col - antPosCol) +
                               abs(state.food[i].row - antPosRow));

        if (newFoodDistance < currentFoodDistance)
        {
            closestFood = state.food[i];
        }
    }

    const_cast<State&>(state).bug << "Closest food is " << closestFood.row << ":" << closestFood.col << std::endl;
    const_cast<State&>(state).bug << "Exit Ant::_findClosestFood()" << std::endl;

    return closestFood;
}

Location Ant::takeDecision(const State &state, double timeLimit)
{
    Location nextMove;
    Location randomLocation;

    const_cast<State&>(state).bug << "Ant::takeDecision()" << std::endl;

    switch (state.playstyle)
    {
    // Style de jeu du début de partie (Moins de 10 fourmis)
    // Fuir les ennemis ou chercher de la nourriture ou explorer
    case PLAYSTYLE_FLEE:

        // Si un ennemi est proche, on fuit
        for (int i = 0; i < state.enemyAnts.size(); i++) {
            if (state.distance(_position, state.enemyAnts[i]) < state.attackradius+2) {
                // Différence de x et y pour trouver la direction de l'ennemi
                int diffX = _position.col - state.enemyAnts[i].col;
                int diffY = _position.row - state.enemyAnts[i].row;

                // On trouve la Location opposée
                Location oppositeLocation = Location(_position.row+diffY, _position.col+diffX);
                if(!state.isLocationReachable(oppositeLocation)) {
                    // Si on ne peut pas aller à l'endroit opposé, on retourne à la fourmilière
                    oppositeLocation = state.myHills[0];
                }
                _setDestination(state, oppositeLocation);

                return _path[0];
            }
        }

        // Si pas d'ennemi proche, on cherche de la nourriture
        nextMove = _tryToFindFood(state);
        if(nextMove != Location(-1, -1)) {
            return nextMove;
        }

        // Si pas de nourriture, on explore
        nextMove = _tryToExplore(state);
        if(nextMove != Location(-1, -1)) {
            return nextMove;
        }

        return _path[0]; // Ne devrait pas arriver, mais on sait jamais
        break;

    // Style de jeu du milieu de partie
    // Chercher de la nourriture ou explorer
    case PLAYSTYLE_EAT:
        // On cherche en priorité de la nourriture
        nextMove = _tryToFindFood(state);
        if(nextMove != Location(-1, -1)) {
            return nextMove;
        }

        // Si pas de nourriture, on explore
        nextMove = _tryToExplore(state);
        if(nextMove != Location(-1, -1)) {
            return nextMove;
        }

        return _path[0]; // Ne devrait pas arriver, mais on sait jamais
        break;
    
    // Style de jeu de la fin de partie
    // On récolte la bouffe, ou on défonce du nid ou on explore
    case PLAYSTYLE_ANIHILATE:
        // On cherche en priorité de la nourriture
        nextMove = _tryToFindFood(state);
        if(nextMove != Location(-1, -1)) {
            const_cast<State&>(state).bug << "Ant::takeDecision() -> Anihilate : Food found" << std::endl;
            return nextMove;
        }

        // Attaque des nids
        if(state.enemyHills.size() > 0) {
            nextMove = _tryToAnihilate(state);
            if(nextMove != Location(-1, -1)) {
                return nextMove;
            }
        }
        // Si pas de nids, on explore
        nextMove = _tryToExplore(state);
        if(nextMove != Location(-1, -1)) {
            return nextMove;
        }

        return _path[0]; // Ne devrait pas arriver, mais on sait jamais
        break;
    default:
    return _path[0]; // N'a aucune chance d'arriver, juste un garde-fou
        break;
    }

    const_cast<State&>(state).bug << "Exit Ant::takeDecision()" << std::endl;
    return Location(-1, -1);
}

void Ant::_makeMove(State& state, int direction) {
    const_cast<State&>(state).bug << "Ant::_makeMove()" << std::endl;
    const_cast<State&>(state).bug << "Current location " << _position.row << ":" << _position.col << std::endl;

    Location destination = state.getLocation(_position, direction);
    const_cast<State&>(state).bug << "Destination " << destination.row << ":" << destination.col << std::endl;

    // Si on se dirige vers une case occupée par une fourmi à nous
    if(!state.isLocationSafe(destination)) {
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

Location Ant::_tryToFindFood(const State& state) {
    const_cast<State&>(state).bug << "Ant::_tryToFindFood()" << std::endl;
    Location closestFood = _findClosestFood(state);
    if(closestFood == Location(-1, -1)) {
        return Location(-1, -1);
    }

    if(_destination == closestFood) {
        const_cast<State&>(state).bug << "Ant::_tryToFindFood() : Already on the way to food" << std::endl;
        return _path[0];
    } else {
        _setDestination(state, closestFood);
        const_cast<State&>(state).bug << "Ant::_tryToFindFood() : New destination" << std::endl;
        const_cast<State&>(state).bug << "Ant::_tryToFindFood() : next move = " << _path[0].row << ":" << _path[0].col << std::endl;
        return _path[0];
    }
}

Location Ant::_tryToExplore(const State& state) {
    const_cast<State&>(state).bug << "Ant::_tryToExplore()" << std::endl;
    // Si on est encore loin de la destination actuelle, on continue
    if(_path.size() > 3) {
        return _path[0];
    }

    // Sinon on explore un autre endroit
    Location randomLocation = Location(-1, -1);

    while (randomLocation == Location(-1, -1)) {
        int randomRow = _position.row + (rand() % 20) - 10;
        int randomCol = _position.col + (rand() % 20) - 10;
        if (state.isLocationReachable(Location(randomRow, randomCol)) && Location(randomRow, randomCol) != _position) {
            randomLocation = Location(randomRow, randomCol);
        }
    }

    _setDestination(state, randomLocation);
    return _path[0];
}

Location Ant::_tryToAnihilate(const State& state) {
    const_cast<State&>(state).bug << "Ant::_tryToAnihilate()" << std::endl;
    // Attaque des nids
    if(state.enemyHills.size() > 0) {
        Location closestHill = state.enemyHills[0];
        int closestHillDistance = state.distance(_position, closestHill);

        for (int i = 0; i < state.enemyHills.size(); i++) {
            int currentHillDistance = state.distance(_position, state.enemyHills[i]);
            if (currentHillDistance < closestHillDistance) {
                closestHill = state.enemyHills[i];
                closestHillDistance = currentHillDistance;
            }
        }

        if(_destination == closestHill) {
            return _path[0];
        } else {
            _setDestination(state, closestHill); // FIXME: Supprimer de la mémoire quand on a détruit le nid
            return _path[0];
        }
    }
    return Location(-1, -1);
}
