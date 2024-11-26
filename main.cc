#include "block.h"
#include "board.h"
#include "observer.h"
#include "textObserver.h"
#include "graphicObserver.h"
#include "tile.h"
#include "game.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    Board *board1 = new Board();
    Board *board2 = new Board();
    Game *game = new Game(0, 1, board1, board2);
    std::vector<Observer*> observers; // Keep a vector of Observer pointers to be deleted at the end
    TextObserver *textObserver = new TextObserver(game);
    GraphicObserver *graphicObserver = new GraphicObserver(game);
    game->attach(textObserver);
    game->attach(graphicObserver);
    // IBlock *testIBlock = new IBlock();
    std::shared_ptr<IBlock> testIBlock1 = std::make_shared<IBlock>();
    std::shared_ptr<IBlock> testIBlock2 = std::make_shared<IBlock>();
    std::shared_ptr<IBlock> testIBlock3 = std::make_shared<IBlock>();
    std::shared_ptr<TBlock> testTBlock1 = std::make_shared<TBlock>();
    std::shared_ptr<TBlock> testTBlock2 = std::make_shared<TBlock>();
    std::shared_ptr<OBlock> testOBlock = std::make_shared<OBlock>();
    std::shared_ptr<StarBlock> testStarBlock = std::make_shared<StarBlock>();
    board1->setNewCurrentBlock(testIBlock1);
    board2->setNewNextBlock(testOBlock);
    board1->setNewNextBlock(testOBlock);
    board1->placeBlock();
    board1->dropBlock();
    board2->setNewCurrentBlock(testIBlock3);
    board2->placeBlock();
    board2->dropBlock();
    board1->setNewCurrentBlock(testTBlock2);
    // board1->dropStarBlock();
    board1->placeBlock();
    board1->dropBlock();
    game->notifyObservers();
    
    delete board1;
    delete board2;
    delete textObserver;
    delete graphicObserver;
    delete game;
    /*
    board1->rotateBlock("CW");
    board1->dropBlock();
    board1->setNewCurrentBlock(testIBlock);
    board1->rotateBlock("CW");
    board1->moveBlock("d");
    board1->moveBlock("d");
    board1->moveBlock("d");
    board1->moveBlock("r");
    board1->moveBlock("r");
    board1->moveBlock("r");
    board1->rotateBlock("CW");
    board1->dropBlock();
    game->notifyObservers();
    */
    // board1->moveBlock(testIBlock, "d");
    // game->notifyObservers();
    // board1->rotateBlock(testIBlock, "CW");
    // game->notifyObservers();
    // board1->moveBlock(testIBlock, "d");
}
