#include <stdlib.h>

#include "Bot.h"
#include "Timer.h"

using namespace std;

int selectDirection(int ant, State& ref_state, double timeLimit);

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


int selectDirection(int ant, State& ref_state, double timeLimit){
    // De quoi s'assurer qu'on ne timeout pas
    Timer timer = Timer();
    timer.start();

    // Variables pour la direction et la destination
    Location nLoc;
    int direction;

    // Algo de choix de direction
    do {
        direction = rand() % 4;
        nLoc = ref_state.getLocation(ref_state.myAnts[ant], direction);

        ref_state.bug << timer.getTime() << "ms < " << timeLimit << "ms" << endl;
    } while (
        timer.getTime() < timeLimit &&
        (
            ref_state.grid[nLoc.row][nLoc.col].isWater ||
            ref_state.grid[nLoc.row][nLoc.col].ant != -1 ||
            ref_state.grid[nLoc.row][nLoc.col].hillPlayer == 0
        )
    );
    // Condition de non-validation du mouvement :
    // Il y a de l'eau
    // Il y a une fourmi
    // C'est notre fourmilière
    
    return direction;
}




/*
4 phases de jeu :

DISCOVERING :
    Condition :
        < 5 fourmis
    
    Répartition :
        1+ fourmis exploratrices                  [EXPLORING] 
    
    Comportement :
        Fuite systématique                        [FLEEING]   
        Ramassage de bouffe                       [GATHERING] 

    Sécurité :
        Restent proches de la fourmilière

    Objectif : Créer assez de fourmis pour avoir une garde sécurisée

SLOW_PLAYING :
    Condition :
        >= 5 fourmis
        < 9 fourmis

    Répartition :
        3 fourmis gardes                          [GUARDING]  
        2+ fourmis exploratrices                  [EXPLORING] 
    
    Comportement :
        Ramassage de bouffe                       [GATHERING] 
        Fuite systématique                        [FLEEING]

    Sécurité :
        Une équipe reste collée à la fourmilière

    Objectif : Créer assez de fourmis pour avoir une armée en défendant la fourmilière

MAIN_GAME :
    Condition :
        >= 9 fourmis
        < 18 fourmis

    Répartition :
        3+ fourmis gardes                          [GUARDING] 
        3* fourmis en formation                    [ROAMING]  

    

    Comportement :
        Ramassage de bouffe                        [GATHERING]
        Mise en formation pour combattre           [FORMING] (formation |)

    Sécurité :
        Une équipe reste collée à la fourmilière 

    Objectif : Former des groupes de 3 fourmis pour qu'elles puissent gagner les combats

WINNING :
    Condition :
        >= 18 fourmis

    Répartition :
        3+ fourmis gardes                          [GUARDING] 
        5* fourmis en formation                    [ROAMING]    

    Comportement :
        Toujours en formation                      [FORMING] (formation x)
        Attaque d'ennemis                          [ATTACKING]
        Destruction de fourmilières ennemies       [DESTROYING]

    Objectif : Exterminer




Les répartitions (+) acceuillent les nouveaux arrivants
Lorsqu'il y a assez de recrues pour une équipe (*) on en crée une
*/