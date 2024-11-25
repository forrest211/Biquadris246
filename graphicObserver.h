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
    // Window for this Observer
    std::unique_ptr<Xwindow> window;
    // Pointer to the Game subject
    Game *game;
    // Char version of the grids to check what needs to be redrawn
    std::vector<char> charGrid1;
    std::vector<char> charGrid2;

    const int WINDOW_WIDTH = 35;
    const int WINDOW_HEIGHT = 21;

    int getColorForBlock(char c);

    public:
        GraphicObserver(Game *game); // Ctor
        void notify() override;
        ~GraphicObserver() = default;
        void printNormal();
        void printBlind();
};

#endif
