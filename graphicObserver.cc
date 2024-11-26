#include "graphicObserver.h"
#include "window.h"
#include <iostream>

using namespace std;

// Colour Code:
/*
IBLock: cyan
JBlock: darkblue
LBlock: orange
OBlock: yellow
ZBlock: green
TBlock: purple
SBlock: red
StarBlock: brown
blindedness: black(?)
*/

// Implementation file for GraphicObserver

GraphicObserver::GraphicObserver(Game *game):game{game} {
    window = std::make_unique<Xwindow>(10*WINDOW_WIDTH, 10*WINDOW_HEIGHT);
    charGrid1.resize(ROWS, std::vector<char>(COLS));
    charGrid2.resize(ROWS, std::vector<char>(COLS));
    // Set all to blank initially
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            charGrid1[i][j] = ' ';
            charGrid2[i][j] = ' '; // Set both grids initially to blank
        }
    }
}

// Notify
void GraphicObserver::notify() {
    printNormal();
}

// Print normally
void GraphicObserver::printNormal() {
    for (int i = 0; i < ROWS; ++i) {
        // Grid1
        for (int j = 0; j < COLS; ++j) {
            char c = game->getState(1,i,j);
            // If it was the same symbol as before, skip
            if (c == charGrid1[i][j]) {
                continue;
            }
            else {
                int colour = getColourForBlock(c);
                window->fillRectangle(j * 10, i * 10, 10, 10, colour);
                charGrid1[i][j] = c; // Update the new char
            }
        }
        // Grid2
        for (int j = 0; j < COLS; ++j) {
            char c = game->getState(2,i,j);
            // If it was the same symbol as before, skip
            if (c == charGrid2[i][j]) {
                continue;
            }
            else {
                int colour = getColourForBlock(c);
                window->fillRectangle((j + GRID2LEFT) * 10, i * 10, 10, 10, colour);
                charGrid2[i][j] = c; // Update the new char
            }
        }
    }
}

// Print with blinded effect
void GraphicObserver::printBlind() {
    for (int i = 0; i < ROWS; ++i) {
        // Grid1
        for (int j = 0; j < COLS; ++j) {
            if (j >= BLINDL && j <= BLINDR && i >= BLINDT && i <= BLINDB) {
                window->fillRectangle(j * 10, i * 10, 10, 10, Xwindow::Black);
            }
            char c = game->getState(1,i,j);
            // If it was the same symbol as before, skip
            if (c == charGrid1[i][j]) {
                continue;
            }
            else {
                int colour = getColourForBlock(c);
                window->fillRectangle(j * 10, i * 10, 10, 10, colour);
                charGrid1[i][j] = c; // Update the new char
            }
        }
        // Grid2
        for (int j = 0; j < 11; ++j) {
            if (j >= BLINDL && j <= BLINDR && i >= BLINDT && i <= BLINDB) {
                window->fillRectangle((j + GRID2LEFT) * 10, i * 10, 10, 10, Xwindow::Black);
            }
            char c = game->getState(2,i,j);
            // If it was the same symbol as before, skip
            if (c == charGrid2[i][j]) {
                continue;
            }
            else {
                int colour = getColourForBlock(c);
                window->fillRectangle((j + GRID2LEFT) * 10, i * 10, 10, 10, colour);
                charGrid2[i][j] = c; // Update the new char
            }
        }
    }
}

// Helper to get the Color
int GraphicObserver::getColourForBlock(char c) {
    switch (c) {
        case 'I': return Xwindow::Cyan;
        case 'J': return Xwindow::DarkBlue;
        case 'L': return Xwindow::Orange;
        case 'O': return Xwindow::Yellow;
        case 'Z': return Xwindow::Green;
        case 'T': return Xwindow::Purple;
        case 'S': return Xwindow::Red;
        case '*': return Xwindow::Brown;
        case ' ': return Xwindow::White;
        case '?': return Xwindow::Black;
        default: return Xwindow::White; // Default color for unexpected cases
    }
}
