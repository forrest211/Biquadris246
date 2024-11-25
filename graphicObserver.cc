#include "graphicObserver.h"
#include "window.h"
#include <iostream>

using namespace std;

// Colour Code:
/*
IBLock: babyBlue
JBlock: darkBlue
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
}

// Notify
void GraphicObserver::notify() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 11; ++j) {
            char c = game->getState(1, i,j);
            int color = getColorForBlock(c);
            window->fillRectangle(j * 10, i * 10, 10, 10, color);
        }
    }
}

// Helper to get the Color
int GraphicObserver::getColorForBlock(char c) {
    switch (c) {
        case 'I': return Xwindow::Red;
        case 'J': return Xwindow::Blue;
        case 'L': return Xwindow::Green;
        case 'O': return Xwindow::Red;
        case 'Z': return Xwindow::Blue;
        case 'T': return Xwindow::Green;
        case 'S': return Xwindow::Red;
        case '*': return Xwindow::Blue;
        case ' ': return Xwindow::White;
        default: return Xwindow::Black; // Default color for unexpected cases
    }
}
