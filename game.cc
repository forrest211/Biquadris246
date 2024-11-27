#include <iostream>
#include "game.h"
#include "board.h"

Game::Game(bool textOnly, int seed, string seq0, string seq1, int startLevel)
    : textOnly{textOnly}, heavySpecAct{false}, hiScore{0}, currPlayerIdx{0} {
    // setting up the players
    p0 = std::make_unique<Player>(0, seq0, startLevel);
    p1 = std::make_unique<Player>(1, seq1, startLevel);
    currPlayerPointer = p0.get();
    // setting up the board for each player, though they do not actually have
    // access to their associated player
    board0 = std::make_unique<Board>();
    board1 = std::make_unique<Board>();
    // initializing the command interpreter
    // ci = std::make_unique<CommandInterpreter>();
}

// Get the state of one of the Boards
char Game::getState(int currPlayerIdx, int row, int col) const {
    if (currPlayerIdx == 0) {
        return board0->charAt(row, col);
    } else {
        return board1->charAt(row, col);
    }
}

Block* Game::getNextBlock(int player) {
    return (player == 0) ? board0->nextBlock.get() : board1->nextBlock.get();
}

int Game::getLevel(int player) const {
    return (player == 0) ? p0->getLevel() : p1->getLevel();
}

int Game::getScore(int player) const {
    return (player == 0) ? p0->getScore() : p1->getScore();
}

void Game::updateHiScore() { hiScore = max(hiScore, max(p0->getScore(), p1->getScore())); }

void Game::updateScoreDestroyedBlock(int origLvl) {
    currPlayerPointer->scoreBlock(origLvl);
    updateHiScore();
}

int Game::getPlayerTurn() const {
    return currPlayerIdx;
}

bool Game::switchPlayerTurn() {
    // clearing any special actions active on our current player
    // clearSpecAct();

    // Clear any full rows and update score
    int rowsCleared = getBoard()->clearFullRows();
    currPlayerPointer->scoreRow(rowsCleared);
    // updating the hi score when the turn ends
    updateHiScore();
    std::cout << "HISCORE" << hiScore << std::endl;
    // updating Player's Blocks for the next turn, displaying the changes to the
    // Board, and determining whether they have lost
    bool playerLost = updateBlock();

    // updating the Player pointer
    if (currPlayerIdx == 0) currPlayerPointer = p1.get();
    else currPlayerPointer = p0.get();

    // updating the player 'index'
    currPlayerIdx = 1 - currPlayerIdx;

    return playerLost;
}

bool Game::updateBlock() {
    getBoard()->setNewCurrentBlock(getBoard()->nextBlock);
    getBoard()->setNewNextBlock(createBlock(currPlayerPointer->getBlock()));

    bool success = getBoard()->tryPlaceBlock();

    // if it is possible to place the Block in its initial position, we place it
    // and display the changes
    if (success) {
        getBoard()->placeBlock();
        notifyObservers();
    }

    // since we wish to return whether the current player has lost, we return
    // the opposite of whether the initial Block placement was successful
    return !success;
}

Board* Game::getBoard() const {
    return currPlayerIdx == 0 ? board0.get() : board1.get();
}

void Game::restart() {
    currPlayerIdx = 0;
    p0->restart();
    p1->restart();
    currPlayerPointer = p0.get();
    board0 = std::make_unique<Board>();
    board1 = std::make_unique<Board>();
    // clearSpecAct();
}

// Tries to drop a 1-by-1 block in the middle column of the current player's board.
// Returns True if successful, and False otherwise (the player loses, since the
// middle column is full and cannot take an extra block)
bool Game::addPenalty() {
    if (currPlayerIdx == 0) return board0->dropStarBlock(currPlayerPointer); // ->addPenaltyBlock();
    else return board1->dropStarBlock(currPlayerPointer); //->addPenaltyBlock();
}

// prompts the current player if they cleared more than 1 row this turn to pick
// one or more special actions, depending on the number of rows cleared
/*
std::vector<std::string> Game::promptForSpecAct(int rowsCleared) {
    const int numOfSpecAct = rowsCleared - SPECIAL_ACTION_THRES;
    std::vector<std::string> validInputSpecAct;

    if (numOfSpecAct > 0) {
        cout << "Multiple rows cleard!" << " You are allowed to pick " << numOfSpecAct;

        // different output depending on the number of special actions the player
        // can pick
        if (numOfSpecAct > 1) cout << " special actions.\n";
        else cout << " special action.\n";

        // listing out the available actions
        cout << "The available actions are:\t"
             << "\tblind\n" << "\theavy\n" << "\tforce BLOCK"
             << ", replace BLOCK by one of I, J, L, O, S, Z, or T\n";
        
        while (validInputSpecAct.size() != numOfSpecAct) {
            promptValSpecAct(validInputSpecAct);
            checkDupSpecAct(validInputSpecAct);
        }
    }

    return validInputSpecAct;
}
*/


/*
void Game::promptValSpecAct(std::vector<std::string>& specActs) {
    while (!ci->parseSpecAct(specActs)) {
        cout << "Invalid special action. Try again.\n";
    }
}
*/

/*
void Game::checkDupSpecAct(std::vector<std::string>& specActs) {
    bool removedDup = false;

    // looking for duplicates by brute force, luckily there cannot be many special
    // actions at a time, technically at most 3 since as the tallest block, the
    // I-block, allows at most 4 rows to be cleared, which is at most 3 special
    // actions
    for (auto it1 = specActs.begin(); it1 != specActs.end(); ++it1) {
        for (auto it2 = std::next(it, 1); it2 != specActs.end();) {
            if (*it1 == *it2){
                specActs.erase(it2);
                removedDup = true;
            } else if ((*it1).size() == 1 && (*it2).size() == 1) {
                specActs.erase(it2);
            }
        }
    }

    if (removedDup) std::cout << "Removed duplicate special actions.\n";
}
*/


// overall method to play the game, uses helper functions for different parts of
// the game, such as a turn, end of a turn, and so on
void Game::play() {
    int currTurnRowsCleared = 0;
    bool currPlayLose = false;

    // setting up both Board for the first turn
    gameInit();
    notifyObservers();
    while (true) {
        playTurn(currTurnRowsCleared, currPlayLose);
        switchPlayerTurn(); // Clears any special actions, updates the score, and updates the next Block
        
    }
    /*
    // storing the active special actions for the next player
    std::vector<std::string> activeSpecActs;

    // the 'while' loop condition plays the turn, while the loop content performs
    // most of the end of turn mechanics not done by 'playTurn()', such as
    // prompting for special actions
    while (playTurn(currTurnRowsCleared, currPlayLose, activeSpecActs)) {
        activeSpecActs = promptForSpecAct(currTurnRowsCleared);

        // Two ways the current Player can lose:
        // 1. The player had 'force' imposed on them, causing them to lose.
        // 2. During Level 4, the 1 by 1 block cannot be dropped upon the player
        //    incurring the Level 4 penalty ('turnEnd()' returns True when the
        //    current player incurs the Level 4 penalty and 'getBoard()->dropStarBlock()' returns 
        //    False when the 1 by 1 block was not dropped successfully, meaning
        //    that column was full prior to the block's addition, and adding
        //    another one caused the current player to lose),
        // 3. Upon exiting their turn and setting up for when they can play
        //    again, their next dropped Block cannot be placed.
        if (currPlayLose ||
            (currPlayerPointer->turnEnd(currTurnRowsCleared) && !getBoard()->dropStarBlock()) ||
            switchPlayerTurn()) {
            bool gameRestart = checkForGameReset();

            // If the player(s) do wish to restart the game, then we do so, and
            // continue this 'while' loop. Otherwise, we simply break out of it,
            // and terminate the program.
            if (gameRestart) {
                currPlayLose = false;
                restart();
                continue;
            } else break;
        }
        
        // reset the number of rows cleared for the next player's turn
        currTurnRowsCleared = 0;
    }
    */
    
}

void Game::gameInit() {
    board0->setNewCurrentBlock(createBlock(p0->getBlock()));
    board0->placeBlock();
    board0->setNewNextBlock(createBlock(p0->getBlock()));
    currPlayerPointer = p1.get();
    board1->setNewCurrentBlock(createBlock(p1->getBlock()));
    board1->placeBlock();
    board1->setNewNextBlock(createBlock(p1->getBlock()));
    currPlayerPointer = p0.get();
}

// Most of the mechanics for a player's turn. Some of the things done by this
// method:
// - Applying any Heavy properties, whether it be from Level or a special action
// - Updating the number of rows cleared by the Player this turn.
// This method runs essentially until EOF or the Player executes a 'drop' command,
// or one of the 'heavy' properties force the player's block to be dropped.

// Only updates on move commands. If on a command it switches to the other player, let other functions
// handle the printing (because we might need to clear rows)
bool Game::playTurn(int& rowsCleared, bool& currPlayerLose) {// , std::vector<std::string> specActs) {
    std::string command;
    // playing through the turn until we either get EOF or the 'drop' command
    while (cin >> command) { // ci->parseCommand(command)) {
        // immediately end the turn, and return to the main playing loop to execute
        // end of turn mechanics
        if (command == "l") {
            getBoard()->moveBlock("l");
            notifyObservers();
        }
        if (command == "r") {
            getBoard()->moveBlock("r");
            notifyObservers();
        }
        if (command == "d") {
            getBoard()->moveBlock("d");
            notifyObservers();
        }
        if (command == "cw") {
            getBoard()->rotateBlock("CW");
            notifyObservers();
        }
        if (command == "ccw") {
            getBoard()->rotateBlock("CCW");
            notifyObservers();
        }
        if (command == "drop") {
            getBoard()->dropBlock(); // We will notify the Observers after calculating clears
            break;
        }
        if (command == "lup") {
            currPlayerPointer->levelUp();
            notifyObservers();
        }
        if (command == "ldown") {
            currPlayerPointer->levelDown();
            notifyObservers();
        }

        

        // in the case where we have the 'restart' command executed, we must also
        // break out of the loop
        if (command == "restart") {
            restart();
            break;
        }

        // ADD THE CASE WHERE WE CHANGE THE BLOCK TO A NEW ONE (FOR TESTING PURPOSES,
        // THOUGH COULD STILL CAUSE A LOSS TECHNICALLY)

        // if we have a moving command other than 'drop', we must possibly apply
        // one of or both of the Heavy properties
        /*
        if (isMovingCom(command) && !applyLevelHeavy()) {
            notifyDisplays();
            break;
        }
        */
        
    }

    // if we did not reach EOF (the player explicitly called "drop" or one of the
    // Heavy properties forced their turn to end), we display the updated end of
    // turn Boards
    /*
    if (command == "drop") {
        // storing the number of rows cleared during this turn
        rowsCleared = getBoard()->clearFullRows();
        std::cout << "CLEARED " << rowsCleared << std::endl;
    }
    */
    
    
    // as long as it is not EOF ('command' is the empty string), then we can
    // continue playing
    return command == "";
}



// tells the text and graphical observers to display the Boards, when appropriate
/*
void Game::notifyDisplays() {
    // notify text display by default

    // if the game is not in text-only, we also open/update the graphical display
    if (!textOnly) {}
}
*/


/*
bool Game::checkForGameReset() {
    // prompt text and graphical (if applicable) observers to display a Game Won
    // message, the only acceptable inputs are Y and N

    std::string s;

    while (ci->checkForGameReset(s)) {
        if (s == "yes" || s == "restart") return true;
        else if (s == "no") return false;
    }

    // EOF without obtaining a valid input (if any), by default we quit the Game
    return false;
}
*/


std::shared_ptr<Block> Game::createBlock(char block) {
    if (block == 'T') return make_shared<TBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else if (block == 'J') return make_shared<JBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else if (block == 'L') return make_shared<LBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else if (block == 'O') return make_shared<OBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else if (block == 'S') return make_shared<SBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else if (block == 'Z') return make_shared<ZBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
    else return make_shared<IBlock>(currPlayerPointer->getLevel(), currPlayerPointer);
}

void Subject::attach(Observer* o) {
    // Add the observer pointer to the back of the vector
    observers.push_back(o);
}

void Subject::detach(Observer* o) {
    // Find the observer and erase it (it does nothing if not found)
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

void Subject::notifyObservers() {
    // Notify each observer in the vector
    for (auto it : observers) {
        it->notify();
    }
}
