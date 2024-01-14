#include "Ant.h"
#include "State.h"

Ant::Ant(int id, Location location)
{
    this->id = id;
    _position = location;
    _pathfinder = AStar();
}

Location Ant::getPosition()
{
    return _position;
}

Location Ant::getTarget()
{
    return _target;
}

void Ant::playTurn(State &state, double timeLimit)
{
    const_cast<State &>(state).bug << "ant " << id << " is playing" << std::endl;
    int direction = selectDirection(state, timeLimit);

    if (direction == -1)
    {
        const_cast<State &>(state).bug << "ant " << id << " goes nowhere" << std::endl;
        return;
    }

    const_cast<State &>(state).bug << "ant " << id << " goes " << direction << std::endl;
    state.makeMove(_position, direction);
}

void Ant::setTarget(const State &state, Location location)
{
    _target = location;
    _pathfinder.pathfind(state, _position, _target);
    _path = _pathfinder.getPath();
}

int Ant::selectDirection(const State &state, double timeLimit)
{
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
        const_cast<State &>(state).bug << "lookingLocation: " << lookingLocation.col << "," << lookingLocation.row << std::endl;
        const_cast<State &>(state).bug << "nLoc: " << nLoc.col << "," << nLoc.row << std::endl;
        if (lookingLocation == nLoc)
        {
            break;
        }
    }

    const_cast<State &>(state).bug << "ant " << id << " goes " << direction << std::endl;

    return direction;
}

Location Ant::findClosestFood(const State &state)
{
    // TODO : calculer la distance en fonction du AStar, pas à vol d'oiseau
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

    return closestFood;
}

Location Ant::takeDecision(const State &state, double timeLimit)
{
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
                if(state.grid[oppositeLocation.row][oppositeLocation.col].isWater) { // TODO: Remplacer par isLocationValid
                    // Si on ne peut pas aller à l'endroit opposé, on essaye de pathfind en dehors de la vue.
                    oppositeLocation = Location(_position.row+diffY*5, _position.col-diffX*5);
                }

                return oppositeLocation;
            }
        }

        // Si pas d'ennemi proche, on cherche de la nourriture
        Location closestFood = findClosestFood(state);
        if (closestFood != Location(-1, -1)) {
            if(_target == closestFood) {
                _path.erase(_path.begin());
                return _path[0];
            } else {
                setTarget(state, closestFood);
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
        Location randomLocation = Location(-1, -1);
        while (randomLocation == Location(-1, -1)) {
            int randomRow = _position.row + (rand() % 10) - 5;
            int randomCol = _position.col + (rand() % 10) - 5;
            if (!state.grid[randomRow][randomCol].isWater) { // TODO: Remplacer par isLocationValid
                randomLocation = Location(randomRow, randomCol);
            }
        }
        setTarget(state, randomLocation);
        return _path[0];
        break;

    // Style de jeu du milieu de partie
    // TODO : Chercher de la nourriture ou explorer
    case PLAYSTYLE_EAT:
        /* code */
        break;
    
    // Style de jeu de la fin de partie
    // TODO : Nourriture si très proche, sinon attaquer les nids
    case PLAYSTYLE_ANIHILATE:
        /* code */
        break;
    default:
        break;
    }

    return Location(-1, -1);
}