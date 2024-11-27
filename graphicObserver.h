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
    const int WINDOW_WIDTH = 50;
    const int WINDOW_HEIGHT = 25;
    const int GRID2LEFT = 16; // How many tiles Grid2 is shifted
    const int P0_IDX = 0, P1_IDX = 1;
    // Window for this Observer
    std::unique_ptr<Xwindow> window = nullptr;
    // Pointer to the Game subject
    Game *game;
    // Char version of the grids to check what needs to be redrawn
    std::vector<std::vector<char>> charGrid1;
    std::vector<std::vector<char>> charGrid2;

    int getColourForBlock(char c);
    void print();

    public:
        GraphicObserver(Game *game); // Ctor
        void notify() override;
        ~GraphicObserver() = default;
};

#endif
