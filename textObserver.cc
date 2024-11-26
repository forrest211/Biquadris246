#include "textObserver.h"
#include <iostream>
using namespace std;

TextObserver::TextObserver(Game *game):game{game}{}

void TextObserver::notify() {
    // Header
    out << "Level:    " << game->getLevel(1) << "     "
        << "Level:    " << game->getLevel(2) << endl;
    out << "Score:    " << game->getScore(1) << "     "
        << "Score:    " << game->getScore(2) << endl;
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
                if (((game->getNextBlock(1))->getCoords())[k].first == j && ((game->getNextBlock(1))->getCoords())[k].second == i) {
                    out << (game->getNextBlock(1))->getBlockSymbol();
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
                if (((game->getNextBlock(2))->getCoords())[k].first == j && ((game->getNextBlock(2))->getCoords())[k].second == i) {
                    out << (game->getNextBlock(2))->getBlockSymbol();
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
            out << game->getState(1, i, j); // State of p1
        }
        cout << "     ";
        for (int j = 0; j < COLS; ++j) {
            out << game->getState(2, i, j); // State of p2
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
                out << game->getState(1, i, j); // State of p1
            }
        }
        cout << "     ";
        for (int j = 0; j < COLS; ++j) {
            if (j >= BLINDL && j <= BLINDR && i >= BLINDT && i <= BLINDB) {
                out << '?';
            }
            else {
                out << game->getState(2, i, j); // State of p2
            }
        }
        out << endl;
    }
}


