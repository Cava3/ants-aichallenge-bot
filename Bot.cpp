#include <stdlib.h>

#include "Ant.h"
#include "AStar.h"
#include "Bot.h"
// #include "State.h"

// using namespace std;

//constructor
Bot::Bot() {
};

//plays a single game of Ants.
void Bot::playGame() {
    //reads the game parameters and sets up
    std::cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(std::cin >> state)
    {
        state.updateVisionInformation();
        state.addFromMemory();
        state.updateMemory();

        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves() {
    state.bug << "turn " << state.turn << ":" << std::endl;
    state.bug << state << std::endl;

    // Pour chaque fourmi (son index dans la liste)
    for(int ant=0; ant<(int)state.myAnts.size(); ant++) {
        double timePerAnt = state.turntime/(int)state.myAnts.size();
        state.myAnts[ant].playTurn(state, timePerAnt);
        // int direction = state.myAnts[ant].selectDirection(state, timePerAnt);

        // if(direction == -1) {
        //     const_cast<State&>(state).bug << "ant " << ant << " goes nowhere" << endl;
        //     continue;
        // }

        // const_cast<State&>(state).bug << "ant " << ant << " goes " << direction << endl;
        // state.makeMove(state.myAnts[ant].getPosition(), direction);
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << std::endl << std::endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    std::cout << "go" << std::endl;
};


// int Bot::selectDirection(int ant, const State& state, double timeLimit){

//     // Variables pour la direction et la destination
//     AStar pathfinder = AStar();
//     Location nLoc;
//     int direction;


//     // On pathfind vers la nourriture la plus proche
//     Location closestFood = findClosestFood(ant, state);
//     pathfinder.pathfind(state, state.myAnts[ant].getPosition(), closestFood);
//     vector<Location> path = pathfinder.getPath();

//     if(path.size() == 0) {
//         const_cast<State&>(state).bug << "/!\\ Empty path" << endl;
//         return -1;
//     }

//     nLoc = path[0];

//     // On détermine la direction en fonction de la position
//     for(direction=0; direction<TDIRECTIONS; direction++) {
//         Location lookingLocation = state.getLocation(state.myAnts[ant].getPosition(), direction);
//         const_cast<State&>(state).bug << "lookingLocation: " << lookingLocation.col << "," << lookingLocation.row << endl;
//         const_cast<State&>(state).bug << "nLoc: " << nLoc.col << "," << nLoc.row << endl;
//         if(lookingLocation == nLoc) {
//             break;
//         }
//     }

//     const_cast<State&>(state).bug << "ant " << ant << " goes " << direction << endl;

//     return direction;
// }

// Location Bot::findClosestFood(int ant, const State& state){
//     // TODO : calculer la distance en fonction du AStar, pas à vol d'oiseau
//     Location closestFood = state.food[0];

//     for(int i = 1; i < state.food.size(); i++){
//         int antPosCol = state.myAnts[ant].getPosition().col;
//         int antPosRow = state.myAnts[ant].getPosition().row;
//         int currentFoodDistance = (
//             abs(closestFood.col - antPosCol) +
//             abs(closestFood.row - antPosRow)
//             );
//         int newFoodDistance = (
//             abs(state.food[i].col - antPosCol) +
//             abs(state.food[i].row - antPosRow)
//             );
        
//         if (newFoodDistance < currentFoodDistance){
//             closestFood = state.food[i];
//         }
//     }

//     return closestFood;
// }
