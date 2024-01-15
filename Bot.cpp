#include <stdlib.h>

#include "Ant.h"
#include "AStar.h"
#include "Bot.h"


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
        try {
            state.updateVisionInformation();
            state.addFromMemory();
            const_cast<State&>(state).bug << "PlayStyle : " << state.playstyle << std::endl;

        
            makeMoves();

            state.updateMemory();
            endTurn();
        } catch (const std::exception& e) {
            const_cast<State&>(state).bug << "Exception : " << e.what() << std::endl;
        }
    }
};

//makes the bots moves for the turn
void Bot::makeMoves() {
    state.bug << "turn " << state.turn << ":" << std::endl;
    state.bug << state << std::endl;

    state.bug << "myAnts : " << state.myAnts.size() << std::endl;
    // Pour chaque fourmi (son index dans la liste)
    for(int ant=0; ant<(int)state.myAnts.size(); ant++) {
        double timePerAnt = state.turntime/(int)state.myAnts.size();
        // int direction = selectDirection(ant, state, timePerAnt);

        // if(direction == -1) {
        //     const_cast<State&>(state).bug << "ant " << ant << " goes nowhere" << std::endl;
        //     continue;
        // }

        // const_cast<State&>(state).bug << "ant " << ant << " goes " << direction << std::endl;
        // state.makeMove(state.myAnts[ant], direction);

        state.myAnts[ant].playTurn(state, timePerAnt);
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << std::endl << std::endl;
};

//finishes the turn
void Bot::endTurn() {
    state.bug << "End turn" << std::endl;
    if(state.turn > 0)
        state.reset();

    state.turn++;

    std::cout << "go" << std::endl;
};
