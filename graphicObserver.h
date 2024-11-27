#ifndef _GRAPHICOBSERVER__H_
#define _GRAPHICOBSERVER__H_
#include "observer.h"
#include "game.h"
#include "board.h"
#include "window.h"
#include <iostream>
#include <vector>
#include <memory>

// Graphic observer
class GraphicObserver: public Observer {
    const int ROWS = 18, COLS = 11;
    const int BLINDL = 2, BLINDR = 8, BLINDT = 2, BLINDB = 11;
    const int PLAYER0 = 0, PLAYER1 = 1;
    const int WINDOW_WIDTH = 50;
    const int WINDOW_HEIGHT = 25;
    const int GRID1LEFT = 16; // How many tiles Grid2 is shifted
    // Window for this Observer
    std::unique_ptr<Xwindow> window = nullptr;
    // Pointer to the Game subject
    Game *game;
    // Char version of the grids to check what needs to be redrawn
    std::vector<std::vector<char>> charGrid0;
    std::vector<std::vector<char>> charGrid1;

    int getColourForBlock(char c);

    public:
        GraphicObserver(Game *game); // Ctor
        void notify() override;
        ~GraphicObserver() = default;
        void printNormal();
        void printBlind();
};

#endif
