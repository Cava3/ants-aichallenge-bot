#pragma once

#include "State.h"

/*
    struct pour le bot dans le jeu. Implémentation dans Bot.cpp
*/
struct Bot
{
    State state;

    Bot();

    void playGame();    // Lancement d'une partie
    void makeMoves();   // Lance le déplacement pour chaque fourmi
    void endTurn();     // Fin du tour
};
