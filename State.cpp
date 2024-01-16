#include <algorithm>

#include "State.h"


//constructor
State::State() {
    gameover = 0;
    turn = 0;
    antsId = 0;
    bug.open("./debug.txt");
}

//sets the state up
void State::setup() {
    grid = std::vector<std::vector<Square>>(rows, std::vector<Square>(cols, Square()));
}

//resets all non-water squares to land and clears the bots ant vector
void State::reset() {
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    food.clear();
    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!grid[row][col].isWater)
                grid[row][col].reset();
}

//outputs move information to the engine
void State::makeMove(const Location& loc_ref, int direction) {
    std::cout << "o " << loc_ref.row << " " << loc_ref.col << " " << CDIRECTIONS[direction] << std::endl;

    Location nLoc = getLocation(loc_ref, direction);

    // On vérifie qu'on ne stocke pas d'information fausse
    if(!grid[nLoc.row][nLoc.col].isWater) {
        grid[nLoc.row][nLoc.col].ant = grid[loc_ref.row][loc_ref.col].ant;
        grid[loc_ref.row][loc_ref.col].ant = -1;
    }
}

//returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location& loc1_ref, const Location& loc2_ref) const {
    int d1 = abs(loc1_ref.row-loc2_ref.row),
        d2 = abs(loc1_ref.col-loc2_ref.col),
        dr = std::min(d1, rows-d1),
        dc = std::min(d2, cols-d2);

    return sqrt(dr*dr + dc*dc);
}

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location& loc_ref, int direction) const {
    int nLocRow = (loc_ref.row + DIRECTIONS[direction][0] + rows) % rows;
    int nLocCol = (loc_ref.col + DIRECTIONS[direction][1] + cols) % cols;

    return Location(nLocRow, nLocCol);
}

bool State::isLocationReachable(const Location& loc_ref) const {
    // LA Location doit être dans la grille
    if(loc_ref.row < 0 || loc_ref.row >= rows || loc_ref.col < 0 || loc_ref.col >= cols)
        return false;

    // La Location ne doit pas être de l'eau
    if(grid[loc_ref.row][loc_ref.col].isWater)
        return false;

    return true;
}

bool State::isLocationSafe(const Location& loc_ref) {
    if(grid[loc_ref.row][loc_ref.col].ant == 0) {
        return false;
    }

    return !grid[loc_ref.row][loc_ref.col].isFood;
}

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation() {
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a = 0; a < (int)myAnts.size(); a++) {
        sLoc = myAnts[a].getPosition();
        locQueue.push(sLoc);

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, 0));
        grid[sLoc.row][sLoc.col].isVisible = 1;
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty()) {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)  {
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && distance(sLoc, nLoc) <= viewradius) {
                    grid[nLoc.row][nLoc.col].isVisible = 1;
                    locQueue.push(nLoc);
                }

                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
}

// Puts back remembered waters on the map
void State::addFromMemory() {
    for(std::vector<Location>::iterator it = waters.begin(); it != waters.end(); ++it)
        if(!grid[it->row][it->col].isVisible)
            grid[it->row][it->col].isWater = true;

    for(std::vector<Location>::iterator it = enemyHills.begin(); it != enemyHills.end(); ++it)
        if(!grid[it->row][it->col].isVisible)
            grid[it->row][it->col].isHill = true;
}

// Remembers the waters from the map
void State::updateMemory() {
    waters.clear();
    enemyHills.clear();

    for(int row=0; row<rows; row++) {
        for(int col=0; col<cols; col++) {
            if(grid[row][col].isWater)
                waters.push_back(Location(row, col));
            else if(grid[row][col].isHill)
                enemyHills.push_back(Location(row, col));
        }
    }
}

// Update playstyle
// TODO: Anti-ghosting
void State::updatePlaystyle() {
    if(myAnts.size() < 10) {
        playstyle = PLAYSTYLE_FLEE;
    }
    else if(myAnts.size() < 20) {
        playstyle = PLAYSTYLE_EAT;
    }
    else {
        playstyle = PLAYSTYLE_ANIHILATE;
    }
}

// Iterate the myAnts vector to return the ant matching the given location
Ant* State::findAnt_ptr(const Location pos, int turn) {
    // 0 for current turn, 1 for next one

    Ant* ant_ptr = NULL;

    for(int i = 0; i < myAnts.size(); i++) {
        if(turn == 0) {
            if(pos == myAnts[i].getPosition()) {
                ant_ptr = &myAnts[i];
                break;
            }
        }
        else if (turn == 1) {
            if(pos == myAnts[i].getNextTurnPosition()) {
                ant_ptr = &myAnts[i];
                break;
            }
        }
    }

    return ant_ptr;
}

// Removes an ant from myAnts vector
void State::deleteAnt(int id) {
    myAnts.erase(
        std::remove_if(
            myAnts.begin(),
            myAnts.end(),
            [id](const Ant &ant)
            {
                return ant.id == id;
            }
        ),
        myAnts.end()
    );
}


/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
std::ostream& operator<<(std::ostream& os_ref, const State& state_ref) {
    for(int row = 0; row < state_ref.rows; row++)  {
        for(int col = 0; col < state_ref.cols; col++) {
            if(state_ref.grid[row][col].isWater)
                os_ref << '%';
            else if(state_ref.grid[row][col].isFood)
                os_ref << '*';
            else if(state_ref.grid[row][col].isHill)
                os_ref << (char)('A' + state_ref.grid[row][col].hillPlayer);
            else if(state_ref.grid[row][col].ant >= 0)
                os_ref << (char)('a' + state_ref.grid[row][col].ant);
            else if(state_ref.grid[row][col].isVisible)
                os_ref << '.';
            else
                os_ref << '?';
        }
        os_ref << std::endl;
    }

    return os_ref;
}

//input function
std::istream& operator>>(std::istream& is_ref, State& state_ref) {
    int row, col, player;
    std::string inputType, junk;

    //finds out which turn it is
    while(is_ref >> inputType) {
        if(inputType == "end") {
            state_ref.gameover = 1;
            break;
        }
        else if(inputType == "turn") {
            is_ref >> state_ref.turn;
            break;
        }
        else //unknown line
            getline(is_ref, junk);
    }

    if(state_ref.turn == 0) {
        //reads game parameters
        while(is_ref >> inputType)  {
            if(inputType == "loadtime")
                is_ref >> state_ref.loadtime;
            else if(inputType == "turntime")
                is_ref >> state_ref.turntime;
            else if(inputType == "rows")
                is_ref >> state_ref.rows;
            else if(inputType == "cols")
                is_ref >> state_ref.cols;
            else if(inputType == "turns")
                is_ref >> state_ref.turns;
            else if(inputType == "player_seed")
                is_ref >> state_ref.seed;
            else if(inputType == "viewradius2") {
                is_ref >> state_ref.viewradius;
                state_ref.viewradius = sqrt(state_ref.viewradius);
            }
            else if(inputType == "attackradius2") {
                is_ref >> state_ref.attackradius;
                state_ref.attackradius = sqrt(state_ref.attackradius);
            }
            else if(inputType == "spawnradius2") {
                is_ref >> state_ref.spawnradius;
                state_ref.spawnradius = sqrt(state_ref.spawnradius);
            }
            else if(inputType == "ready") { //end of parameter input
                state_ref.timer.start();
                break;
            }
            else    //unknown line
                getline(is_ref, junk);
        }
    }
    else {
        //reads information about the current turn
        while(is_ref >> inputType) {
            if(inputType == "w") { //water square
                is_ref >> row >> col;
                state_ref.grid[row][col].isWater = 1;
            }
            else if(inputType == "f") { //food square
                is_ref >> row >> col;
                state_ref.grid[row][col].isFood = 1;
                state_ref.food.push_back(Location(row, col));
            }
            else if(inputType == "a") { //live ant square
                is_ref >> row >> col >> player;
                state_ref.grid[row][col].ant = player;
                if(player == 0) {
                    // Looking for ants whose move has successfully been processed
                    // by matching ant._nextTurnPosition
                    Ant* ant_ptr = state_ref.findAnt_ptr(Location(row, col), 1);
                    
                    if(ant_ptr != NULL) {
                        ant_ptr->validateLastTurnMove(state_ref, true);
                    }
                    else {
                        // Looking for ants whose move has not been processed
                        // (either blocked by engine or new ant)
                        ant_ptr = state_ref.findAnt_ptr(Location(row, col), 0);

                        if(ant_ptr != NULL) {
                            const_cast<State&>(state_ref).bug << "Ant " << ant_ptr->id << " did not move last turn" << std::endl;
                            ant_ptr->validateLastTurnMove(state_ref, false);
                        }
                        else {
                            // New ant from the hill
                            state_ref.antsId++;
                            state_ref.myAnts.push_back(Ant(state_ref.antsId, state_ref, Location(row, col)));
                        }
                    }
                }
                else
                    state_ref.enemyAnts.push_back(Location(row, col));
            }
            else if(inputType == "d") { //dead ant square
                is_ref >> row >> col >> player;
                state_ref.grid[row][col].deadAnts.push_back(player);

                if(player == 0) {
                    Ant* ant_ptr = state_ref.findAnt_ptr(Location(row, col), 1);

                    if(ant_ptr != NULL) {
                        const_cast<State&>(state_ref).bug << "Ant " << ant_ptr->id << " of player " << player << " dead at " << row << ":" << col << std::endl;
                        state_ref.deleteAnt(ant_ptr->id);
                    }
                }
            }
            else if(inputType == "h") {
                is_ref >> row >> col >> player;
                state_ref.grid[row][col].isHill = 1;
                state_ref.grid[row][col].hillPlayer = player;
                if(player == 0)
                    state_ref.myHills.push_back(Location(row, col));
                else
                    state_ref.enemyHills.push_back(Location(row, col));

            }
            else if(inputType == "players") //player information
                is_ref >> state_ref.noPlayers;
            else if(inputType == "scores") { //score information
                state_ref.scores = std::vector<double>(state_ref.noPlayers, 0.0);
                for(int p=0; p<state_ref.noPlayers; p++)
                    is_ref >> state_ref.scores[p];
            }
            else if(inputType == "go") { //end of turn input
                if(state_ref.gameover)
                    is_ref.setstate(std::ios::failbit);
                else
                    state_ref.timer.start();
                break;
            }
            else //unknown line
                getline(is_ref, junk);
        }

        // Update playstyle
        state_ref.updatePlaystyle();
    }

    return is_ref;
}
