#include "player.h"
#include <iostream>

Player::Player(int playerIdx, std::string s, int startLevel):playerIdx{playerIdx},score{0},seq{s} {
    setLevel(startLevel);
    score = 0; // Set score to 0
    lvl4LastClearRow = 0;
    pol = std::make_unique<ProbsOfLevels>();
}

void Player::setLevel(int level) {
    if (level < LOWEST_LVL || level > HIGHEST_LVL) return;
    else if (level == 0) l = std::make_unique<Level0>(level, seq, seq);
    else l = std::make_unique<ProbsLevel>(level, pol->obtainLvlProb(level));
}

// Increase the level of the player
void Player::levelUp() {
    int currLevel = getLevel();
    currLevel++;
    if (currLevel <= HIGHEST_LVL) {
        l = std::make_unique<ProbsLevel>(currLevel, pol->obtainLvlProb(currLevel));
    }
    // If it exceeds highest_lvl, it does nothing
}

// Increase the level of the player
void Player::levelDown() {
    int currLevel = getLevel();
    currLevel--;
    if (currLevel >= LOWEST_LVL) {
        l = std::make_unique<ProbsLevel>(currLevel, pol->obtainLvlProb(currLevel));
    }
    // If it exceeds highest_lvl, it does nothing
}

int Player::getScore() const { return score; }

int Player::getLevel() const { return l->getLevel(); }

// void Player::setNoRand(std::string s) { l->setNoRand(s); }

// void Player::setRand() { l->setRand(); }

char Player::getBlock() const { return l->produceBlock(playerIdx); }

void Player::addSpecAct(bool lastWholeTurn, std::string specAct) {
    activeSpecAct.push_back({lastWholeTurn, specAct});
}

void Player::clearSpecAct() { activeSpecAct.clear(); }

void Player::restart() {
    score = 0;
    setLevel(getLevel());
    clearSpecAct();
    lvl4LastClearRow = 0;
}

void Player::scoreRow(int rowsCleared) {
    if (rowsCleared) {
        score += (l->getLevel() + rowsCleared) * (l->getLevel() + rowsCleared);
    }
}

void Player::scoreBlock(int origLvl) {
    std::cout << "prev score: " << score << " origLvl: " << origLvl << std::endl;
    score += (origLvl + 1) * (origLvl + 1);
}

bool Player::turnEnd(int rowsCleared) {
    // increasing the score based on row clearing
    scoreRow(rowsCleared);

    // if we are on Level 4 and we've cleared at least one row, we can reset the
    // penalty counter
    if (l->getLevel() >= 4 && rowsCleared) lvl4LastClearRow = 0;
    // if we are on Level 4 but did not clear a row, we increment the penalty
    // counter
    else if (l->getLevel() >= 4) {
        ++lvl4LastClearRow;

        // if we must add the penalty block, we return True, otherwise False is
        // returned
        if (lvl4LastClearRow % PENALTY_TURNS == 0) return true;
    }

    return false;
}
