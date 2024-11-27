#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <memory>
#include <vector>

#include "board.h"
#include "observer.h"
#include "player.h"
#include "tile.h"
#include "commandInterpreter.h"

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
    // constants indicating the number of rows the block would move down depending
    // one which Heavy applies
    const int HEAVY_LVL_DOWN = 1;
    const int HEAVY_SPEC_ACT_DOWN = 2;
    const int P0_IDX = 0, P1_IDX = 1;
    // 'heavySpecAct' is true when the current board has the Heavy special action
    // applied to it
    bool heavySpecAct;
    int hiScore;
    // update 'currPlayerIdx' using: currPlayerIdx = 1 - currPlayerIdx, like
    // taking the NOT of a bit
    int currPlayerIdx;
    std::unique_ptr<Player> p0, p1;
    int consec_drop0, consec_drop1;
    // raw pointer pointing to the current player, makes it easier to access the
    // actual Player object instead, and relies on 'currPlayerIdx' integer to
    // switch between the two players easily
    Player *currPlayerPointer;
    std::unique_ptr<Board> board0, board1;
    std::unique_ptr<CommandInterpreter> ci;

    // private methods, mechanics to allow our game to run
    void updateHiScore();
    // Switches Player turn. Before switching also determines whether the current
    // Player has lost upon trying to place their next Block onto the Board (true
    // if they lose, false if the initial Block placement is successful)
    bool switchPlayerTurn();
    // this method handles the edge case of having a non-zero or non-one multipler
    // on the 'drop' command, which would make them 'drop' the current Block
    // immediately when it is their turn
    bool handleConsecDrops();
    void setConsecDrops(int multiplier);
    // Returns TRUE when the turn ended successfully, meaning it is now the next
    // player's turn, and FALSE when EOF is reached. Directly mutates the argument
    // to indicate how many rows the player has cleared on their turn. Also
    // indicates whether the player has lost upon having special actions applied.
    bool playTurn(int &currTurnRowsCleared, bool &currPlayerLose, std::vector<std::string> specActs);
    // this method is executed at the very beginning of the game to set up the
    // Boards' initial Blocks properly
    void gameInit();
    // this method is called to update the opponent/next Player's Block, and
    // checks whether it fits onto their Board
    bool updateBlock();
    // Given a command, we check whether we must apply any of the Heavy properties
    // (applies them if needed). Returns True if the current Player's turn has ended,
    // and false otherwise. Also directly mutates the given boolean to indicate
    // whether the current player has lost (a turn ends when a player has lost).
    // Commands that do not update the Board directly, such as norandom, sequence,
    // etc. make this method return false.
    bool updateBoard(std::string command, bool& currPlayerLose);
    // determining whether a command is a moving command, excluding 'drop',
    // meant to be used to check whether we must apply the 'heavy' property of
    // Levels 3 and higher
    bool isMovingCom(const std::string com) const;
    // If applicable, we must apply the 'heavy' property to the moved blocks,
    // returns True if the 'down' command calls were successful, otherwise we
    // return False to indicate that the drop has been automatically dropped
    // without needing the 'drop' command being executed. This is used for both
    // the Level and Special Action 'Heavy'
    bool applyHeavy();
    // prompting the player to choose special action(s) depending on 'rowsCleared'
    std::vector<std::string> promptForSpecAct(int rowsCleared);
    // obtaining valid input from the player when prompting them for special
    // action(s)
    void promptValSpecAct(std::vector<std::string> &validSpecAct);
    // checking for duplicates for the chosen special actions
    void checkDupSpecAct(std::vector<std::string> &specActs);
    // Methods relating to the special actions, adding them to a Board and
    // clearing the special actions upon a turn end, as all special actions
    // currently only last one turn for a player. Returns true if adding the
    // special actions does not cause the affected Player to lose ('force' may
    // cause the Player to lose)
    bool addSpecActs(std::vector<std::string> specActs);
    void clearSpecActs();
    // method to add the penalty 1-by-1 block for the current player if they
    // were unable to clear a block within 5 turns when in Level 4
    bool addPenalty();
    // when a player loses, all commands are locked, and we will prompt the
    // player(s) for whether they wish to restart the game
    bool checkForGameReset();
    // creating Block objects depending on which Block we want, and returning it
    std::shared_ptr<Block> createBlock(const char block);

   public:
    Game(int seed, string seq0, string seq1, int startLevel);  // Ctor
    ~Game() override = default;

    // Accessors (and settors?)
    int getLevel(int player) const;
    int getScore(int player) const;
    void updateScoreDestroyedBlock(int increase);

    int getPlayerTurn() const;
    Player *getCurrentPlayer() const;
    Board *getBoard() const;

    char getState(int board, int row, int col) const override;

    Block *getNextBlock(int p);  // For textObserver to fetch the next Block
    void play();
    void restart();

    // for the observers to determine whether a specific board is blind, 0 means
    // board0, 1 means board1
    bool isBoardBlind(int board);
};

#endif
