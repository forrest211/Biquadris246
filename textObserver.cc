#include "textObserver.h"
#include <iostream>
using namespace std;

TextObserver::TextObserver(Game *game):game{game}{}

void TextObserver::notify() {
    // Header
    out << "Level:    " << game->getLevel(PLAYER0) << "     "
        << "Level:    " << game->getLevel(PLAYER1) << endl;
    out << "Score:    " << game->getScore(PLAYER0) << "     "
        << "Score:    " << game->getScore(PLAYER1) << endl;
    out << "-----------     -----------" << endl;
    
    printNormal();

    // Footer
    out << "-----------     -----------" << endl;
    out << "Next:      " << "     "
        << "Next:      " << endl;
    for (int i = 2; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            bool found = false;
            // If the following does not find, print " "
            for (int k = 0; k < 4; ++k) {
                if (((game->getNextBlock(PLAYER0))->getCoords())[k].first == j && ((game->getNextBlock(PLAYER0))->getCoords())[k].second == i) {
                    out << (game->getNextBlock(PLAYER0))->getBlockSymbol();
                    found = true;
                }
            }
            if (!found) out << " ";
        }
        out << "            ";
        for (int j = 0; j < 4; ++j) {
            bool found = false;
            // If the following does not find, print " "
            for (int k = 0; k < 4; ++k) {
                if (((game->getNextBlock(PLAYER1))->getCoords())[k].first == j && ((game->getNextBlock(PLAYER1))->getCoords())[k].second == i) {
                    out << (game->getNextBlock(PLAYER1))->getBlockSymbol();
                    found = true;
                }
            }
            if (!found) out << " ";
        }
        out << "       " << endl;
    }
}

void TextObserver::printNormal() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            out << game->getState(PLAYER0, i, j); // State of p0
        }
        cout << "     ";
        for (int j = 0; j < COLS; ++j) {
            out << game->getState(PLAYER1, i, j); // State of p1
        }
        out << endl;
    }
}

void TextObserver::printBlind() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (j >= BLINDL && j <= BLINDR && i >= BLINDT && i <= BLINDB) {
                out << '?';
            }
            else {
                out << game->getState(PLAYER0, i, j); // State of p0
            }
        }
        cout << "     ";
        for (int j = 0; j < COLS; ++j) {
            if (j >= BLINDL && j <= BLINDR && i >= BLINDT && i <= BLINDB) {
                out << '?';
            }
            else {
                out << game->getState(PLAYER1, i, j); // State of p1
            }
        }
        out << endl;
    }
}


