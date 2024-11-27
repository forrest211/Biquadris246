#ifndef PLAYER_H
#define PLAYER_H
#include "level0.h"
#include "probsLevel.h"
#include "probsOfLevels.h"
#include <string>
#include <memory>
#include <cmath>

class Player {
    const int LOWEST_LVL = 0, HIGHEST_LVL = 4, PENALTY_TURNS = 5;
    int playerIdx;
    int score;
    // Counter used for the logic of Level 4, and potentially any levels onward
    // that would inherit the properties from Level 4. It indicates the number of
    // turns SPENT IN LEVEL 4 (or potentially higher) WITHOUT CLEARING A ROW, not
    // the actual number of turns elapsed in the game, nor the number of turns
    // spent in Level 4 in total.
    int lvl4LastClearRow;
    std::string seq;
    std::unique_ptr<Level> l;
    std::unique_ptr<ProbsOfLevels> pol;
    // PUT THIS INTO BOARD
    std::vector<std::pair<bool, std::string>> activeSpecAct;

   

    public:
        // ctor
        Player(int playerIdx, std::string s, int startLevel);
        // setter, used by ctor and Game for 'levelup' and 'leveldown' commands
        void setLevel(int levelToSet);
        void levelUp(); // for levelup
        void levelDown(); // for leveldown
        // getter methods
        int getScore() const;
        int getLevel() const;
        // 'norandom FILENAME' command used
        // void setNoRand(std::string s = "");
        // 'random' command used
        // void setRand();
        char getBlock() const;
        // CHANGE THIS SUCH THAT BOARD ITSELF STORES THE SPECIAL ACTION
        void addSpecAct(bool lastWholeTurn, std::string specAct);
        void clearSpecAct();
        // when the 'restart' command is used
        void restart();
        // end of the player's turn
        bool turnEnd(int rowsCleared);
        // scoring based on block clearing
        void scoreBlock(int origLvl);
        // scoring based on row clearing
        void scoreRow(int rowsCleared);
};

#endif
