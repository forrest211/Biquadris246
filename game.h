#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <memory>

#include "board.h"
#include "observer.h"
#include "tile.h"
#include "player.h"
// #include "commandInterpreter.h"

class Observer;  // forward declaration

// Game will be the Subject for the Observers
class Subject {
    std::vector<Observer *> observers;

   public:
    void attach(Observer *o);
    void detach(Observer *o);
    void notifyObservers();
    virtual char getState(int player, int row, int col) const = 0;
    virtual ~Subject() = default;
};

class Game : public Subject {
    // clearing more than 'SPECIAL_ACTION_THRES' rows allows the current player
    // to pick at least one special action
    const int SPECIAL_ACTION_THRES = 1;
    // Level number for which any such Level or higher has the 'heavy' property
    const int HEAVY_LVL = 3;
    // 'heavySpecAct' is true when the current board has the Heavy special action
    // applied to it
    bool textOnly, heavySpecAct;
    int hiScore;
    // update 'currPlayerIdx' using: currPlayerIdx = 1 - currPlayerIdx, like
    // taking the NOT of a bit
    int currPlayerIdx = 0;
    std::unique_ptr<Player> p0, p1;
    // raw pointer pointing to the current player, makes it easier to access the
    // actual Player object instead, and relies on 'currPlayerIdx' integer to
    // switch between the two players easily
    Player* currPlayerPointer;
    std::unique_ptr<Board> board0, board1;
    /*std::unique_ptr<CommandInterpreter> ci;*/ // ADD THIS BACK LATER

    // private methods, mechanics to allow our game to run
    void updateHiScore();
    // Switches Player turn. Before switching also determines whether the current
    // Player has lost upon trying to place their next Block onto the Board (true
    // if they lose, false if the initial Block placement is successful)
    bool switchPlayerTurn();
    // Returns TRUE when the turn ended successfully, meaning it is now the next
    // player's turn, and FALSE when EOF is reached. Directly mutates the argument
    // to indicate how many rows the player has cleared on their turn. Also
    // indicates whether the player has lost upon having special actions applied.
    bool playTurn(int& currTurnRowsCleared, bool& currPlayerLose); // , std::vector<std::string> specActs);
    // this method is executed at the very beginning of the game to set up the
    // Boards' initial Blocks properly
    void gameInit();
    // this method is called to update the opponent/next Player's Block, and
    // checks whether it fits onto their Board
    bool updateBlock();
    // this method immediately updates the current Block of the Board to become
    // the specified block, used for testing purposes and the 'force' special
    // action
    // void setCurrBlock(const char block);
    // determining whether a command is a moving command, excluding 'drop',
    // meant to be used to check whether we must apply the 'heavy' property of
    // Levels 3 and higher
    // bool isMovingCom(const std::string com);
    // if applicable, we must apply the 'heavy' property to the moved blocks,
    // returns True if the 'down' command calls were successful, otherwise we
    // return False to indicate that the drop has been automatically dropped
    // without needing the 'drop' command being executed
    // bool applyLevelHeavy();
    // when this method is called, we notify the appropriate observers to display
    // the board
    // void notifyDisplays();
    // prompting the player to choose special action(s) depending on 'rowsCleared'
    // std::vector<std::string> promptForSpecAct(int rowsCleared);
    // obtaining valid input from the player when prompting them for special
    // action(s)

    // void promptValSpecAct(std::vector<std::string>& validSpecAct);

    // checking for duplicates for the chosen special actions

    // void checkDupSpecAct(std::vector<std::string>& specActs);

    // Methods relating to the special actions, adding them to a Board and
    // clearing the special actions upon a turn end, as all special actions
    // currently only last one turn for a player. Returns true if adding the
    // special actions does not cause the affected Player to lose ('force' may
    // cause the Player to lose)
    // bool applySpecAct(std::vector<std::string> specActs);
    // void clearSpecAct();
    // method to add the penalty 1-by-1 block for the current player if they
    // were unable to clear a block within 5 turns when in Level 4
    bool addPenalty();
    // when a player loses, all commands are locked, and we will prompt the
    // player(s) for whether they wish to restart the game
    // bool checkForGameReset();
    // creating Block objects depending on which Block we want, and returning it
    std::shared_ptr<Block> createBlock(const char block);

   public:
    Game(bool textOnly, int seed, string seq0, string seq1, int startLevel);  // Ctor

    // Accessors (and settors?)
    int getLevel(int player) const;
    int getScore(int player) const;
    void updateScoreDestroyedBlock(int increase);

    int getPlayerTurn() const;
    Board *getBoard() const;

    char getState(int currPlayerIdx, int row, int col) const override;
    
    Block *getNextBlock(int p); // For textObserver to fetch the next Block
    void play();
    void restart();
};

#endif