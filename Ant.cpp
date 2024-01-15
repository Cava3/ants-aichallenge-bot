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

void Ant::_setDestination(const State& state, Location location) {
    const_cast<State&>(state).bug << "Ant::_setDestination()" << std::endl;
    _destination = location;
    _pathfinder.pathfind(state, _position, _destination);
    _path = _pathfinder.getPath();

    if(_path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << std::endl;
    }
}

int Ant::_selectDirection(const State &state, double timeLimit)
{
    const_cast<State&>(state).bug << "Ant::_selectDirection()" << std::endl;
    Location nLoc;
    int direction;

    // On choisis quelle action faire
    nLoc = takeDecision(state, timeLimit);

    if (_path.size() == 0)
    {
        const_cast<State &>(state).bug << "/!\\ Empty path" << std::endl;
        return -1;
    }


    // On détermine la direction en fonction de la position
    for (direction = 0; direction < TDIRECTIONS; direction++)
    {
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

    return closestFood;
}

Location Ant::takeDecision(const State &state, double timeLimit)
{
    Location closestFood;
    Location randomLocation;

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
                if(state.grid[oppositeLocation.row][oppositeLocation.col].isWater) { // FIXME: Remplacer par isLocationValid
                    // Si on ne peut pas aller à l'endroit opposé, on essaye de pathfind en dehors de la vue.
                    oppositeLocation = Location(_position.row+diffY*5, _position.col-diffX*5);
                }

                return oppositeLocation;
            }
        }

        // Si pas d'ennemi proche, on cherche de la nourriture
        closestFood = _findClosestFood(state);
        if (closestFood != Location(-1, -1)) {
            if(_destination == closestFood) {
                _path.erase(_path.begin());
                return _path[0];
            } else {
                _setDestination(state, closestFood);
                return _path[0];
            }
        }

        // Si pas de nourriture, on explore
        // Si on est encore loin de la destination actuelle, on continue
        if (_path.size() > 3) {
            _path.erase(_path.begin());
            return _path[0];
        }

        // Sinon on explore un autre endroit
        randomLocation = Location(-1, -1);
        while (randomLocation == Location(-1, -1)) {
            int randomRow = _position.row + (rand() % 10) - 5;
            int randomCol = _position.col + (rand() % 10) - 5;
            if (!state.grid[randomRow][randomCol].isWater) { // FIXME: Remplacer par isLocationValid
                randomLocation = Location(randomRow, randomCol);
            }
        }
        _setDestination(state, randomLocation);
        return _path[0];
        break;

    // Style de jeu du milieu de partie
    // Chercher de la nourriture ou explorer
    case PLAYSTYLE_EAT:
        // On cherche en priorité de la nourriture
        closestFood = _findClosestFood(state);
        if (closestFood != Location(-1, -1)) {
            if(_destination == closestFood) {
                _path.erase(_path.begin());
                return _path[0];
            } else {
                _setDestination(state, closestFood);
                return _path[0];
            }
        }

        // Si pas de nourriture, on explore
        // Si on est encore loin de la destination actuelle, on continue
        if (_path.size() > 3) {
            _path.erase(_path.begin());
            return _path[0];
        }

        // Sinon on explore un autre endroit
        randomLocation = Location(-1, -1);
        while (randomLocation == Location(-1, -1)) {
            int randomRow = _position.row + (rand() % 10) - 5;
            int randomCol = _position.col + (rand() % 10) - 5;
            if (!state.grid[randomRow][randomCol].isWater) { // TODO: Remplacer par isLocationValid
                randomLocation = Location(randomRow, randomCol);
            }
        }
        _setDestination(state, randomLocation);
        return _path[0];
        break;
    
    // Style de jeu de la fin de partie
    // Nourriture si très proche, sinon attaquer les nids
    case PLAYSTYLE_ANIHILATE:
        // On cherche en priorité de la nourriture
        // TODO: Proche
        closestFood = _findClosestFood(state);
        if (closestFood != Location(-1, -1)) {
            if(_destination == closestFood) {
                _path.erase(_path.begin());
                return _path[0];
            } else {
                _setDestination(state, closestFood);
                return _path[0];
            }
        }

        // Si pas de nourriture, on attaque les nids
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
                _path.erase(_path.begin());
                return _path[0];
            } else {
                _setDestination(state, closestHill); // FIXME: Supprimer de la mémoire quand on a détruit le nid
                return _path[0];
            }
        } else {
            // Si pas de nids, on explore
            // Si on est encore loin de la destination actuelle, on continue
            if (_path.size() > 3) {
                _path.erase(_path.begin());
                return _path[0];
            }

            // Sinon on explore un autre endroit
            randomLocation = Location(-1, -1);
            while (randomLocation == Location(-1, -1)) {
                int randomRow = _position.row + (rand() % 10) - 5;
                int randomCol = _position.col + (rand() % 10) - 5;
                if (!state.grid[randomRow][randomCol].isWater) { // TODO: Remplacer par isLocationValid
                    randomLocation = Location(randomRow, randomCol);
                }
            }
            _setDestination(state, randomLocation);
            return _path[0];
        }

        break;
    default:
        break;
    }

    return Location(-1, -1);
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