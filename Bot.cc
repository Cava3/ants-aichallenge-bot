#include <stdlib.h>

#include "Bot.h"
#include "Timer.h"
#include "AStar.hh"


using namespace std;

int selectDirection(int ant, const State& state, double timeLimit);

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
        // TODO: Timer failsafe
        double timePerAnt = state.turntime/(int)state.myAnts.size();
        int direction = selectDirection(ant, state, timePerAnt);
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


int selectDirection(int ant, const State& state, double timeLimit){
    // De quoi s'assurer qu'on ne timeout pas
    Timer timer = Timer();
    timer.start();

    // Variables pour la direction et la destination
    AStar pathfinder = AStar();
    Location nLoc;
    int direction;

    const_cast<State&>(state).bug << "Time taken: " << timer.getTime() << "ms" << endl;

    // On pathfind vers 0,0
    pathfinder.pathfind(state, state.myAnts[ant], Location(9,70));
    vector<Location> path = pathfinder.getPath();
    nLoc = path[0];

    // On détermine la direction en fonction de la position
    for(direction=0; direction<TDIRECTIONS; direction++) {
        if(state.getLocation(state.myAnts[ant], direction) == nLoc) {
            break;
        }
    }

    return direction;
}
