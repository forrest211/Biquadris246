#include "game.h"

#include <iostream>

#include "board.h"

Game::Game(bool bonus, int seed, string seq0, string seq1, int startLevel)
    : bonus{bonus}, heavySpecAct{false}, hiScore{0}, currPlayerIdx{0}, consec_drop0{0}, consec_drop1{0} {
    // setting up the players
    p0 = std::make_unique<Player>(seq0, startLevel);
    p1 = std::make_unique<Player>(seq1, startLevel);
    currPlayerPointer = p0.get();
    // setting up the board for each player, though they do not actually have
    // access to their associated player
    board0 = std::make_unique<Board>();
    board1 = std::make_unique<Board>();
    // initializing the command interpreter
    ci = std::make_unique<CommandInterpreter>();
}

// Get the state of one of the Boards
char Game::getState(int playerIdx, int row, int col) const {
    if (playerIdx == P0_IDX)
        return board0->charAt(row, col);
    else
        return board1->charAt(row, col);
}

Block* Game::getNextBlock(int player) {
    return (player == P0_IDX) ? board0->nextBlock.get() : board1->nextBlock.get();
}

int Game::getLevel(int player) const {
    return (player == P0_IDX) ? p0->getLevel() : p1->getLevel();
}

int Game::getScore(int player) const {
    return (player == P0_IDX) ? p0->getScore() : p1->getScore();
}

int Game::getHiScore() const { return hiScore; }

void Game::updateHiScore() { hiScore = max(hiScore, max(p0->getScore(), p1->getScore())); }

int Game::getPlayerTurn() const {
    return currPlayerIdx;
}

Player* Game::getCurrentPlayer() const {
    return currPlayerPointer;
}

bool Game::switchPlayerTurn() {
    // clearing any special actions active on our current player
    clearSpecActs();
    // updating Player's Blocks for the next turn, displaying the changes to the
    // Board, and determining whether they have lost
    bool playerLost = updateBlock();

    // updating the Player pointer
    if (currPlayerIdx == P0_IDX)
        currPlayerPointer = p1.get();
    else
        currPlayerPointer = p0.get();

    // updating the player 'index'
    currPlayerIdx = 1 - currPlayerIdx;

    return playerLost;
}

bool Game::updateBlock() {
    getBoard()->setNewCurrentBlock(getBoard()->getBoardNextBlock());
    getBoard()->setNewNextBlock(createBlock(currPlayerPointer->getBlock()));

    bool success = getBoard()->tryPlaceBlock();

    // if it is possible to place the Block in its initial position, we place it
    if (success) getBoard()->placeBlock();

    // since we wish to return whether the current player has lost, we return
    // the opposite of whether the initial Block placement was successful
    return !success;
}

std::shared_ptr<Block> Game::createBlock(const char block) {
    Player* p = currPlayerPointer;

    if (block == 'I')
        return make_shared<IBlock>(p->getLevel(), p);
    else if (block == 'J')
        return make_shared<JBlock>(p->getLevel(), p);
    else if (block == 'L')
        return make_shared<LBlock>(p->getLevel(), p);
    else if (block == 'O')
        return make_shared<OBlock>(p->getLevel(), p);
    else if (block == 'S')
        return make_shared<SBlock>(p->getLevel(), p);
    else if (block == 'Z')
        return make_shared<ZBlock>(p->getLevel(), p);
    else
        return make_shared<TBlock>(p->getLevel(), p);
}

Board* Game::getBoard() const {
    return currPlayerIdx == P0_IDX ? board0.get() : board1.get();
}

void Game::restart() {
    currPlayerIdx = P0_IDX;
    currPlayerPointer = p0.get();
    board0 = std::make_unique<Board>();
    board1 = std::make_unique<Board>();
    p0->restart();
    p1->restart();
    clearSpecActs();
    consec_drop0 = 0;
    consec_drop1 = 0;
    gameInit();
}

// Tries to drop a 1-by-1 block in the middle column of the current player's board.
// Returns True if successful, and False otherwise (the player loses, since the
// middle column is full and cannot take an extra block)
bool Game::addPenalty() {
    if (currPlayerIdx == P0_IDX)
        return board0->dropStarBlock(currPlayerPointer);
    else
        return board1->dropStarBlock(currPlayerPointer);
}

// prompts the current player if they cleared more than 1 row this turn to pick
// one or more special actions, depending on the number of rows cleared
std::vector<std::string> Game::promptForSpecAct(int rowsCleared, bool& isEOF) {
    int numOfSpecAct;

    if (bonus) numOfSpecAct = rowsCleared - SPECIAL_ACTION_THRES;
    else if (rowsCleared > 1) numOfSpecAct = 1;
    else numOfSpecAct = 0;

    std::vector<std::string> validInputSpecAct;

    if (numOfSpecAct > 0) {
        cout << "Multiple rows cleared!" << " You are allowed to pick " << numOfSpecAct;

        // different output depending on the number of special actions the player
        // can pick
        if (numOfSpecAct > 1)
            cout << " special actions.\n";
        else
            cout << " special action.\n";

        while (validInputSpecAct.size() != numOfSpecAct) {
            std::string specActPicked;

            if (readFromSeq.is_open()) {
                specActPicked = ci->parseSpecAct(readFromSeq);

                if (specActPicked == sEOF) {
                    readFromSeq.close();
                    std::cout << "Sequence file completed." << std::endl;
                    continue;
                }
            } else {
                specActPicked = ci->parseSpecAct(std::cin);

                if (specActPicked == sEOF) {
                    isEOF = true;
                    return validInputSpecAct;
                }
            }

            if (specActPicked != "") checkDupSpecAct(validInputSpecAct, specActPicked);
        }
    }

    return validInputSpecAct;
}

void Game::checkDupSpecAct(std::vector<std::string>& specActs, std::string toAdd) {
    // looking for duplicates, only one loop is needed due to how we check for
    // duplicates when we try to add 'toAdd' onto 'specActs'
    for (auto specAct : specActs) {
        // If we have the same special action, then we tell the Player and return
        // without adding 'toAdd' to 'specActs'. We handle the 'force' special
        // action not with the 'force' string (like we do with 'blind' and 'heavy'),
        // but instead we directly specify the Block which the Player wishes to
        // force, so a duplicate is detected if there is already a string of
        // length 1 in 'specActs', as 'force' was already given previously.
        if (specAct == toAdd || (specAct.size() == 1 && toAdd.size() == 1)) {
            std::cout << "Removed duplicate special actions.\n";
            return;
        }
    }

    // if we reach here, then 'toAdd' was not a duplicate special action, and we
    // can add it to the active special actions
    specActs.push_back(toAdd);
}

void Game::getPoints(int rowsCleared) {
    currPlayerPointer->scoreRow(rowsCleared);
    updateHiScore();
}

// overall method to play the game, uses helper functions for different parts of
// the game, such as a turn, end of a turn, and so on
void Game::play() {
    int currTurnRowsCleared = 0;
    bool currPlayLose = false;
    bool isEOF = false;
    bool gameReset = false;

    // setting up both Board for the first turn
    gameInit();

    // storing the active special actions for the next player
    std::vector<std::string> activeSpecActs;

    // the 'while' loop condition plays the turn, while the loop content performs
    // most of the end of turn mechanics not done by 'playTurn()', such as
    // prompting for special actions
    while (playTurn(currTurnRowsCleared, currPlayLose, activeSpecActs, gameReset)) {
        if (gameReset) {
            gameReset = false;
            activeSpecActs.clear();
            continue;
        }

        bool playerEndTurn = currPlayerPointer->turnEnd(currTurnRowsCleared);
        bool addingPenaltyCauseLoss = false;

        if (playerEndTurn) {
            addingPenaltyCauseLoss = !addPenalty();
            notifyObservers();

            getPoints(getBoard()->clearFullRows());
        }

        bool switchPlayerCauseLoss = switchPlayerTurn();
        // Two ways the current Player can lose:
        // 1. The player had 'force' imposed on them, causing them to lose.
        // 2. During Level 4, the 1 by 1 block cannot be dropped upon the player
        //    incurring the Level 4 penalty ('turnEnd()' returns True when the
        //    current player incurs the Level 4 penalty and 'addPenalty()' returns
        //    False when the 1 by 1 block was not dropped successfully, meaning
        //    that column was full prior to the block's addition, and adding
        //    another one caused the current player to lose),
        // 3. Upon exiting their turn and setting up for when they can play
        //    again, their next dropped Block cannot be placed.
        if (currPlayLose ||
            (playerEndTurn && addingPenaltyCauseLoss) ||
            switchPlayerCauseLoss) {
            notifyWin();
            
            bool gameRestart = checkForGameReset();

            // If the player(s) do wish to restart the game, then we do so, and
            // continue this 'while' loop. Otherwise, we simply break out of it,
            // and terminate the program.
            if (gameRestart) {
                currPlayLose = false;
                restart();
                continue;
            } else
                break;
        }

        // due to how we read separately from the input stream when prompting
        // for special actions, we must make a separate check to see whether we
        // have reached EOF when trying to obtain special action(s) from the
        // Player, and if so, we terminate the program instead of going to the
        // next turn
        activeSpecActs = promptForSpecAct(currTurnRowsCleared, isEOF);

        if (isEOF) {
            std::cout << "End of input detected. Exiting..." << std::endl;
            return;
        }

        // reset the number of rows cleared for the next player's turn
        currTurnRowsCleared = 0;
    }

    std::cout << "End of input detected. Exiting..." << std::endl;
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

std::string Game::getCommand(std::string& filename) {
    if (readFromSeq.is_open())
        return ci->parseCommand(readFromSeq, filename, bonus);
    else {
        std::cout << "Enter command: ";
        return ci->parseCommand(std::cin, filename, bonus);
    }
}

// Most of the mechanics for a player's turn. Some of the things done by this
// method:
// - Applying any Heavy properties, whether it be from Level or a special action
// - Updating the number of rows cleared by the Player this turn.
// This method runs essentially until EOF or the Player executes a 'drop' command,
// or one of the 'heavy' properties force the player's block to be dropped.
bool Game::playTurn(int& rowsCleared, bool& currPlayerLose, std::vector<std::string> specActs, bool& gameReset) {
    // applying the active special actions, and in the case that 'force' causes
    // the Player to lose, we immediately end the turn, but return true as the
    // player(s) can decide whether to restart
    if (!addSpecActs(specActs)) {
        currPlayerLose = true;
        return true;
    }
    // prompt observers to display the Boards
    notifyObservers();

    // in the case the player decided to drop consecutive blocks by using a
    // multiplier for the 'drop' command, no input is read/needed and their
    // current Block is immediately dropped. The method returns true if there
    // was a call to make consecutive drops, and false if there is no need to
    // directly execute the 'drop' command.
    if (handleConsecDrops()) {
        rowsCleared = getBoard()->clearFullRows();
        getPoints(rowsCleared);
        notifyObservers();

        return true;
    }

    std::string filename;

    std::string commandSeq = getCommand(filename);

    while (true) {
        if (commandSeq == "bonus") {
            if (bonus) {
                std::cout << "Enhancements disabled." << std::endl;
                bonus = false;
            } else {
                std::cout << "Enhancements enabled." << std::endl;
                bonus = true;
            }

            commandSeq = getCommand(filename);
            continue;
        } else if (commandSeq == sEOF && readFromSeq.is_open()) {
            readFromSeq.close();
            std::cout << "Sequence file completed." << std::endl;
            commandSeq = getCommand(filename);
            continue;
        } else if (commandSeq == sEOF) break;
        else if (commandSeq == "") {
            commandSeq = getCommand(filename);
            continue;
        }

        std::istringstream iss{commandSeq};
        std::string curr;

        while (iss >> curr) {
            size_t multIdx = 0;
            int multiplier = 1;

            // getting all the digits of the multiplier
            while (multIdx < curr.size() && std::isdigit(curr[multIdx])) ++multIdx;

            if (multIdx > 0) multiplier = std::stoi(curr.substr(0, multIdx));

            std::string command = curr.substr(multIdx);

            if (command == "drop" && multiplier > 0) {
                setConsecDrops(multiplier - 1);
                getBoard()->dropBlock();
                getBoard()->setNewCurrentBlock(nullptr);

                rowsCleared = getBoard()->clearFullRows();
                getPoints(rowsCleared);

                if (rowsCleared > 1) notifyObservers();

                return true;
            } else if (command == "help" || command == "rename") continue;
            else if (command == "restart") {
                restart();
                gameReset = true;
                return true;
            } else if (command == "norandom")
                currPlayerPointer->setNoRand(filename);
            else if (command == "random")
                currPlayerPointer->setRand();
            else if (command == "sequence")
                readFromSeq.open(filename);
            else if (command == "levelup")
                levelUp(currPlayerIdx, multiplier);
            else if (command == "leveldown")
                levelDown(currPlayerIdx, multiplier);
            // Applies the appropriate Heavy effects if necessary, and displays the
            // changes made to the Board. Upon reading a command, if the multiplier
            // is 0, it means that the Command Interpreter has already executed the
            // command for us, but we must still check whether the command executed
            // may end the turn of the current player without calling the 'drop'
            // command. If the multiplier is -1, it means that the player has added
            // a zero multiplier to their command, so we would do nothing regardless
            // of their command
            else if (multiplier > 0 && updateBoard(command, multiplier, currPlayerLose)) {
                rowsCleared = getBoard()->clearFullRows();
                getPoints(rowsCleared);
                return true;
        }

            notifyObservers();
        }

        commandSeq = getCommand(filename);
    }

    return false;
}

void Game::levelUp(int idx, int multiplier) {
    if (idx == 0) {
        int lvl = p0->getLevel();

        for (int i = 0; i < multiplier; ++i) p0->setLevel(++lvl);
    } else {
        int lvl = p1->getLevel();

        for (int i = 0; i < multiplier; ++i) p1->setLevel(++lvl);
    }
}

void Game::levelDown(int idx, int multiplier) {
    if (idx == 0) {
        int lvl = p0->getLevel();

        for (int i = 0; i < multiplier; ++i) p0->setLevel(--lvl);
    } else {
        int lvl = p1->getLevel();

        for (int i = 0; i < multiplier; ++i) p1->setLevel(--lvl);
    }
}

bool Game::handleConsecDrops() {
    // return true in case where the current Player does automatically drop their
    // current Block, and their turn ends
    if (currPlayerIdx == P0_IDX && consec_drop0 != 0) {
        getBoard()->dropBlock();
        getBoard()->setNewCurrentBlock(nullptr);
        --consec_drop0;
        return true;
    } else if (currPlayerIdx == P1_IDX && consec_drop1 != 0) {
        getBoard()->dropBlock();
        getBoard()->setNewCurrentBlock(nullptr);
        --consec_drop1;
        return true;
    }

    return false;
}

void Game::setConsecDrops(int multiplier) {
    currPlayerIdx == P0_IDX ? consec_drop0 = multiplier : consec_drop1 = multiplier;
}

bool Game::updateBoard(std::string command, int multiplier, bool& currPlayerLose) {
    // the only command that has a length of 1 is when we wish to set the currently
    // undropped Block to the specified Block, but this might make the Player lose
    if (command.size() == 1) {
        getBoard()->removeBlock(true);
        getBoard()->setNewCurrentBlock(createBlock(command[0]));

        // try to place the new selected Block
        if (!getBoard()->tryPlaceBlock()) {
            // before returning, we output the updated Board, which is essentially
            // the Board after removing the old replaced Block
            notifyObservers();
            // the Player's turn has ended due to them losing
            currPlayerLose = true;
            // indicate turn end by returning true
            return true;
        }

        getBoard()->placeBlock();
    } else if (isMovingCom(command))
        return executeMove(command, multiplier);

    // if we get here, either we got an invalid command, represented by an empty
    // string (which would not trigger any of the above statements) or command
    // executed did not end the Player's turn
    return false;
}

bool Game::executeMove(std::string command, int multiplier) {
    int heavyMoves = getLevel(currPlayerIdx) >= HEAVY_LVL ? HEAVY_LVL_DOWN : 0;

    if (command == "left") {
        for (int i = 0; i < multiplier; ++i) getBoard()->moveBlock("l");

        if (heavySpecAct) heavyMoves += HEAVY_SPEC_ACT_DOWN;
    } else if (command == "right") {
        for (int i = 0; i < multiplier; ++i) getBoard()->moveBlock("r");

        if (heavySpecAct) heavyMoves += HEAVY_SPEC_ACT_DOWN;
    } else if (command == "down")
        for (int i = 0; i < multiplier; ++i) getBoard()->moveBlock("d");
    else if (command == "clockwise")
        for (int i = 0; i < multiplier; ++i) getBoard()->rotateBlock("CW");
    else
        for (int i = 0; i < multiplier; ++i) getBoard()->rotateBlock("CCW");

    // apply the Heavy property, if needed
    for (int i = 0; i < heavyMoves; ++i) {
        // in the case that the Block is dropped due to one or both of the
        // Heavy properties, we return true to indicate that the Player's
        // turn has ended before the 'drop' command is executed/given
        if (!applyHeavy()) return true;
    }

    // the given moving command did not end the player's turn
    return false;
}

bool Game::addSpecActs(std::vector<std::string> specActs) {
    for (auto specAct : specActs) {
        if (specAct == "blind")
            getBoard()->setBlind(true);
        else if (specAct == "heavy")
            heavySpecAct = true;
        // case where the special action is force, which may cause a loss for the
        // Player, so we may return before applying all the special actions
        else {
            // remove the currently undropped Block
            getBoard()->removeBlock(true);
            // setting the current Block to the specified one
            getBoard()->setNewCurrentBlock(createBlock(specAct[0]));

            // try to place new specified Block, if we are unsuccessful, it means
            // the Player has lost
            if (!getBoard()->tryPlaceBlock()) return false;

            // otherwise, we place the Block in its starting position
            getBoard()->placeBlock();
        }
    }

    // return true to signal that adding the special actions did not cause the
    // Player to lose
    return true;
}

void Game::clearSpecActs() {
    getBoard()->setBlind(false);
    heavySpecAct = false;
}

bool Game::isMovingCom(const std::string command) const {
    return (command == "left" || command == "right" || command == "down" ||
            command == "clockwise" || command == "counterclockwise");
}

bool Game::applyHeavy() {
    // if it is not possible to move the Block, there is no need to call
    // the actual moving method, and we return false to signal that the
    // Block is dropped
    if (!getBoard()->tryMoveBlock("d")) return false;
    // otherwise, we can simply move the Block down by one row, and return
    // true to signal that the Block is not dropped due to the Level Heavy
    else {
        getBoard()->moveBlock("d");
        return true;
    }
}

bool Game::isBoardBlind(int board) {
    if (board == P0_IDX)
        return board0->isBlind();
    else
        return board1->isBlind();
}

bool Game::checkForGameReset() {
    // prompt text and graphical (if applicable) observers to display a Game Won
    // message, the only acceptable inputs are Y and N

    std::string s, f;

    if (readFromSeq.is_open()) {
        s = getCommand(f);

        while (s != sEOF) {
            size_t multIdx = 0;

            while (multIdx < s.size() && std::isdigit(s[multIdx])) ++multIdx;

            std::string command = s.substr(multIdx);

            if (command == "restart") return true;

            s = getCommand(f);
        }

        std::cout << "Sequence file completed." << std::endl;
    }

    readFromSeq.close();

    s = getCommand(f);

    while (s != sEOF) {
        size_t multIdx = 0;

        while (multIdx < s.size() && std::isdigit(s[multIdx])) ++multIdx;

        std::string command = s.substr(multIdx);

        if (command == "restart") return true;

        s = getCommand(f);
    }

    // EOF without obtaining a valid input (if any), by default we quit the Game
    return false;
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

void Subject::notifyWin() {
    // Notify each observer in the vector
    for (auto it : observers) {
        it->notifyWin();
    }
}
