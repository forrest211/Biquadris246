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
    charGrid1.resize(15, std::vector<char>(11));
    charGrid2.resize(15, std::vector<char>(11));
    // Set all to blank initially
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 11; ++j) {
            charGrid1[i][j] = ' ';
            charGrid2[i][j] = ' '; // Set both grids initially to blank
        }
    }
}

// Notify
void GraphicObserver::notify() {
    printBlind();
}

// Print normally
void GraphicObserver::printNormal() {
    for (int i = 0; i < 15; ++i) {
        // Grid1
        for (int j = 0; j < 11; ++j) {
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
    for (int i = 0; i < 15; ++i) {
        // Grid1
        for (int j = 0; j < 11; ++j) {
            if (j >= 2 && j <= 8 && i >= 2 && i <= 11) {
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
            if (j >= 2 && j <= 8 && i >= 2 && i <= 11) {
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
