#include <stdlib.h>

#include "Bot.h"
#include "AStar.hh"


using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
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
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

    // Pour chaque fourmi (son index dans la liste)
    for(int ant=0; ant<(int)state.myAnts.size(); ant++) {
        double timePerAnt = state.turntime/(int)state.myAnts.size();
        int direction = selectDirection(ant, state, timePerAnt);

        if(direction == -1) {
            const_cast<State&>(state).bug << "ant " << ant << " goes nowhere" << endl;
            continue;
        }

        const_cast<State&>(state).bug << "ant " << ant << " goes " << direction << endl;
        state.makeMove(state.myAnts[ant], direction);
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};


int Bot::selectDirection(int ant, const State& state, double timeLimit){

    // Variables pour la direction et la destination
    AStar pathfinder = AStar();
    Location nLoc;
    int direction;


    // On pathfind vers 0,0
    pathfinder.pathfind(state, state.myAnts[ant], Location(51,65));
    vector<Location> path = pathfinder.getPath();

    if(path.size() == 0) {
        const_cast<State&>(state).bug << "/!\\ Empty path" << endl;
        return -1;
    }

    nLoc = path[0];

    // On détermine la direction en fonction de la position
    for(direction=0; direction<TDIRECTIONS; direction++) {
        Location lookingLocation = state.getLocation(state.myAnts[ant], direction);
        const_cast<State&>(state).bug << "lookingLocation: " << lookingLocation.col << "," << lookingLocation.row << endl;
        const_cast<State&>(state).bug << "nLoc: " << nLoc.col << "," << nLoc.row << endl;
        if(lookingLocation == nLoc) {
            break;
        }
    }

    const_cast<State&>(state).bug << "ant " << ant << " goes " << direction << endl;

    return direction;
}
