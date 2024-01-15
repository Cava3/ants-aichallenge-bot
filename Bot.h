#pragma once

#include "State.h"

// Phases de jeu
#define DISCOVERING 0
#define SLOW_PLAYING 1
#define MAIN_GAME 2
#define WINNING 3

// Actions de fourmis / groupes
#define EXPLORING 0  // Exploration de la carte
#define GATHERING 1  // Ramassage des food
#define FLEEING 2    // Fuite d'un enemis
#define GUARDING 3   // Garde de la fourmilière
#define ROAMING 4    // Déplacement en formation
#define FORMING 5    // Mise en formation de combat
#define ATTACKING 6  // Attaque d'un enemis
#define DESTROYING 7 // Destruction d'une fourmilière

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    Bot();

    void playGame();    //plays a single game of Ants
    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves
};
