#include <stdlib.h>

#include "Ant.h"
#include "AStar.h"
#include "Bot.h"


Bot::Bot() {
};

// Lancement d'une partie
void Bot::playGame() {
    // Initialisation de l'état
    std::cin >> state;
    state.setup();
    endTurn();

    // Boucle principale : on joue tant qu'on nous donne des informations
    while(std::cin >> state)
    {
        state.updateVisionInformation();
        state.addFromMemory();

        makeMoves();

        state.updateMemory();
        endTurn();
    }
};

// Lance le déplacement pour chaque fourmi
void Bot::makeMoves() {
    state.bug << "turn " << state.turn << ":" << std::endl;
    state.bug << state << std::endl;

    state.bug << "myAnts : " << state.myAnts.size() << std::endl;
    // Pour chaque fourmi (son index dans la liste)
    for(int ant=0; ant<(int)state.myAnts.size(); ant++) {
        double timePerAnt = state.turntime/(int)state.myAnts.size();

        state.myAnts[ant].playTurn(state, timePerAnt); // On la fait jouer
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << std::endl << std::endl;
};

// Fin du tour
void Bot::endTurn() {
    if(state.turn > 0)
        state.reset();

    state.turn++;

    std::cout << "go" << std::endl;
};
