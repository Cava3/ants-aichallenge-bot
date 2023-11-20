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
    Location nLoc;
    int direction;

    // Algo de choix de direction
    do {
        direction = rand() % 4;
        nLoc = state.getLocation(state.myAnts[ant], direction);

        // const_cast car l'opérateur << n'est pas const
        const_cast<State&>(state).bug << timer.getTime() << "ms < " << timeLimit << "ms" << endl;
    } while (
        timer.getTime() < timeLimit &&
        (   // Condition de non-validation du mouvement :
            state.grid[nLoc.row][nLoc.col].isWater ||       // Il y a de l'eau
            state.grid[nLoc.row][nLoc.col].ant != -1 ||     // Il y a une fourmi
            state.grid[nLoc.row][nLoc.col].hillPlayer == 0  // C'est notre fourmilière
        )
    );
    
    
    return direction;
}


