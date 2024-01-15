#include <algorithm>

#include "State.h"


//constructor
State::State()
{
    gameover = 0;
    turn = 0;
    antsId = 0;
    bug.open("./debug.txt");
};

//sets the state up
void State::setup()
{
    grid = std::vector<std::vector<Square>>(rows, std::vector<Square>(cols, Square()));
};

//resets all non-water squares to land and clears the bots ant vector
void State::reset()
{
    // myAnts.clear();
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    food.clear();
    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!grid[row][col].isWater)
                grid[row][col].reset();
};

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction)
{
    // const_cast<State&>(*this).bug << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << std::endl;
    std::cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << std::endl;

    Location nLoc = getLocation(loc, direction);

    // On vérifie qu'on ne stocke pas d'information fausse
    if(!grid[nLoc.row][nLoc.col].isWater) {
        grid[nLoc.row][nLoc.col].ant = grid[loc.row][loc.col].ant;
        grid[loc.row][loc.col].ant = -1;
    }
};

//returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2) const
{
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = std::min(d1, rows-d1),
        dc = std::min(d2, cols-d2);
    return sqrt(dr*dr + dc*dc);
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction) const
{
    int nLocRow = (loc.row + DIRECTIONS[direction][0] + rows) % rows;
    int nLocCol = (loc.col + DIRECTIONS[direction][1] + cols) % cols;

    return Location(nLocRow, nLocCol);
};

bool State::isLocationReachable(const Location& location) const {
    // LA Location doit être dans la grille
    if(location.row < 0 || location.row >= rows || location.col < 0 || location.col >= cols)
        return false;

    // La Location ne doit pas être de l'eau
    if(grid[location.row][location.col].isWater)
        return false;

    return true;
}

bool State::isLocationSafe(const Location& location) {
    if(grid[location.row][location.col].ant == 0) {
        return false;
    }

    return !grid[location.row][location.col].isFood;
}

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation()
{
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a=0; a<(int) myAnts.size(); a++)
    {
        sLoc = myAnts[a].getPosition();
        locQueue.push(sLoc);

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, 0));
        grid[sLoc.row][sLoc.col].isVisible = 1;
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && distance(sLoc, nLoc) <= viewradius)
                {
                    grid[nLoc.row][nLoc.col].isVisible = 1;
                    locQueue.push(nLoc);
                }
                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
};

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
    bug << "Updating memory" << std::endl;
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
    } else if(myAnts.size() < 20) {
        playstyle = PLAYSTYLE_EAT;
    } else {
        playstyle = PLAYSTYLE_ANIHILATE;
    }
}

// Iterate the myAnts vector to return the ant matching the given location
Ant* State::findAnt(const Location pos, int turn) {
    // 0 for current turn, 1 for next one

    Ant* ant = NULL;

    for(int i = 0; i < myAnts.size(); i++) {
        if(turn == 0) {
            if(pos == myAnts[i].getPosition()) {
                ant = &myAnts[i];
                break;
            }
        }
        else if (turn == 1) {
            if(pos == myAnts[i].getNextTurnPosition()) {
                ant = &myAnts[i];
                break;
            }
        }
    }

    return ant;
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
std::ostream& operator<<(std::ostream &os, const State &state)
{
    for(int row=0; row<state.rows; row++)
    {
        for(int col=0; col<state.cols; col++)
        {
            if(state.grid[row][col].isWater)
                os << '%';
            else if(state.grid[row][col].isFood)
                os << '*';
            else if(state.grid[row][col].isHill)
                os << (char)('A' + state.grid[row][col].hillPlayer);
            else if(state.grid[row][col].ant >= 0)
                os << (char)('a' + state.grid[row][col].ant);
            else if(state.grid[row][col].isVisible)
                os << '.';
            else
                os << '?';
        }
        os << std::endl;
    }

    return os;
};

//input function
std::istream& operator>>(std::istream &is, State &state)
{
    int row, col, player;
    std::string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state.gameover = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> state.turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(state.turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> state.loadtime;
            else if(inputType == "turntime")
                is >> state.turntime;
            else if(inputType == "rows")
                is >> state.rows;
            else if(inputType == "cols")
                is >> state.cols;
            else if(inputType == "turns")
                is >> state.turns;
            else if(inputType == "player_seed")
                is >> state.seed;
            else if(inputType == "viewradius2")
            {
                is >> state.viewradius;
                state.viewradius = sqrt(state.viewradius);
            }
            else if(inputType == "attackradius2")
            {
                is >> state.attackradius;
                state.attackradius = sqrt(state.attackradius);
            }
            else if(inputType == "spawnradius2")
            {
                is >> state.spawnradius;
                state.spawnradius = sqrt(state.spawnradius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                state.timer.start();
                break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else
    {
        //reads information about the current turn
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.grid[row][col].isWater = 1;
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;
                state.grid[row][col].isFood = 1;
                state.food.push_back(Location(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                if(player == 0) {
                    // Looking for ants whose move has successfully been processed
                    // by matching ant._nextTurnPosition
                    Ant* ant_ptr = state.findAnt(Location(row, col), 1);
                    
                    if(ant_ptr != NULL) {
                        ant_ptr->validateLastTurnMove(state, true);
                    }
                    else {
                        // Looking for ants whose move has not been processed
                        // (either blocked by engine or new ant)
                        ant_ptr = state.findAnt(Location(row, col), 0);

                        if(ant_ptr != NULL) {
                            const_cast<State&>(state).bug << "Ant " << ant_ptr->id << " did not move last turn" << std::endl;
                            ant_ptr->validateLastTurnMove(state, false);
                        }
                        else {
                            // New ant from the hill
                            state.antsId++;
                            state.myAnts.push_back(Ant(state.antsId, state, Location(row, col)));
                        }
                    }
                }
                else
                    state.enemyAnts.push_back(Location(row, col));
            }
            else if(inputType == "d") //dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].deadAnts.push_back(player);

                if(player == 0) {
                    Ant* ant_ptr = state.findAnt(Location(row, col), 1);

                    if(ant_ptr != NULL) {
                        const_cast<State&>(state).bug << "Ant " << ant_ptr->id << " of player " << player << " dead at " << row << ":" << col << std::endl;
                        state.deleteAnt(ant_ptr->id);
                    }
                }
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if(player == 0)
                    state.myHills.push_back(Location(row, col));
                else
                    state.enemyHills.push_back(Location(row, col));

            }
            else if(inputType == "players") //player information
                is >> state.noPlayers;
            else if(inputType == "scores") //score information
            {
                state.scores = std::vector<double>(state.noPlayers, 0.0);
                for(int p=0; p<state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state.gameover)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }

        // Update playstyle
        state.updatePlaystyle();
    }

    return is;
};


