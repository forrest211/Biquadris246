#include "block.h"
#include "board.h"
#include "observer.h"
#include "textObserver.h"
#include "graphicObserver.h"
#include "tile.h"
#include "game.h"

#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main(int argc, char* argv[]) {
    std::unique_ptr<Game> game = std::make_unique<Game>(false, 0, "", "", 0);
    std::unique_ptr<TextObserver> textObserver = std::make_unique<TextObserver>(game.get());
    // std::unique_ptr<GraphicObserver> graphicObserver = std::make_unique<GraphicObserver>(game.get());
    game->attach(textObserver.get());
    // game->attach(graphicObserver.get());
    game->play();
}
