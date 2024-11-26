#include "board.h"
#include "tile.h"
#include "block.h"
#include <memory>

// Constructor
Board::Board(){
    grid.resize(15, std::vector<Tile>(11));
    // Set all to blank initially
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 11; ++j) {
            Tile blankTile{' ', false, nullptr};
            grid[i][j] = blankTile;
            /*
            if (j % 2 == 0) {
                Tile blankTile{'#', false, nullptr};
                grid[i][j] = blankTile;
            }
            else {
                Tile blankTile{'@', false, nullptr};
                grid[i][j] = blankTile;
            }
            */
        }
    }
}

char Board::charAt(int row, int col) const {
    return grid[row][col].getSymbol();
}

// For the textObserver to get the next Block
Block* Board::getNextBlock() { return nextBlock.get(); }

void Board::setNewCurrentBlock(std::shared_ptr<Block> block) {
    currentBlock = block;
}
void Board::setNewNextBlock(std::shared_ptr<Block> block) {
    nextBlock = block;
}

// Check whether a Block can be placed at the starting position
bool Board::tryPlaceBlock() {
    for (const auto& tile : currentBlock->getCoords()) {
        size_t x = tile.first;
        size_t y = tile.second;
        if (x < 0 || x >= grid[0].size() || y < 0 || y >= grid.size()) {
            return false; // Invalid position on the board
        }
        if (grid[y][x].getIsOccupied()) {
            return false; // Tile already occupied
        }
    }
    return true; // All Tiles not occupied
}
// Place the Block on the Board
void Board::placeBlock() {
    for (const auto& tile : currentBlock->getCoords()) {
        grid[tile.second][tile.first] = currentBlock->getBlockTile(); // Place the new Tile
    }
}

// Remove the Bloack on the Board (does not modify the Block's coordinates)
void Board::removeBlock() {
    for (const auto& tile : currentBlock->getCoords()) {
        grid[tile.second][tile.first] = Tile(' '); // Replace with Blank Tile
    }
}

// Check whether a Block can be rotated
bool Board::tryRotateBlock(string dir) {
    std::vector<std::pair<int, int>> newCoords = currentBlock->computeRotatedCoords(dir); // Obtain the new coords
    for (const auto& tile : newCoords) {
        size_t x = tile.first;
        size_t y = tile.second;
        if (x < 0 || x >= grid[0].size() || y < 0 || y >= grid.size()) {
            return false; // Invalid position on the board
        }
        if (grid[y][x].getIsOccupied()) {
            // Check if the occupied tile was itself
            bool isItself = false;
            for (const auto &oldTile : currentBlock->getCoords()) {
                if (tile == oldTile) {
                    isItself = true;
                }
            }
            if (!isItself) {
                return false; // Tile already occupied
            }
        }
    }
    return true; // All Tiles not occupied
}
// Rotate the Block
void Board::rotateBlock(string dir) {
    if (tryRotateBlock(dir)) {
        removeBlock();
        currentBlock->rotate(dir);
        placeBlock();
    }
}

// Check whether the Block can be moved in specified direction
bool Board::tryMoveBlock(string dir) {
    std::vector<std::pair<int, int>> newCoords = currentBlock->computeMovedCoords(dir); // Obtain the new coords
    for (const auto& tile : newCoords) {
        size_t x = tile.first;
        size_t y = tile.second;
        if (x < 0 || x >= grid[0].size() || y < 0 || y >= grid.size()) {
            return false; // Invalid position on the board
        }
        if (grid[y][x].getIsOccupied()) {
            // Check if the occupied tile was itself
            bool isItself = false;
            for (const auto &oldTile : currentBlock->getCoords()) {
                if (tile == oldTile) {
                    isItself = true;
                }
            }
            if (!isItself) {
                return false; // Tile already occupied
            }
        }
    }
    return true; // All Tiles not occupied
}
// Move the Block (if tryMoveBlock returns true)
void Board::moveBlock(string dir) {
    if (tryMoveBlock(dir)) {
        removeBlock();
        currentBlock->move(dir);
        placeBlock();
    }
}

// Drop the Block
void Board::dropBlock() {
    while (tryMoveBlock("d")) {
        moveBlock("d");
    }
}

// [TODO]
// Clear any full rows and shift above Tile downwards if needed
int Board::clearFullRows() {
    int clearedRows = 0;
    int row = 14;
    while (row > 0) {
        bool isRowFull = true;
        for (int i = 0; i < 11; ++i) {
            if (grid[row][i].getSymbol() == ' ') {
                isRowFull = false;
            }
        }
        // If the row is full, shift all rows above downwards (which will remove the full row Tiles)
        if (isRowFull) {
            shiftDown(row);
            ++clearedRows;
        }
        // Or else move up to a higher row
        else {
            --row;
        }
    }
    return clearedRows;
}

// [TODO]
// Shift all Tiles above and including row i down 1
void Board::shiftDown(int i){
    Tile blankTile{' ', false, nullptr};
    for (int j = i; j > 0; --j) {
        for (int k = 0; k < 11; k++) {
            grid[j][k] = grid[j-1][k];
            grid[j-1][k] = blankTile;
        }
    }
}

void Board::clearBoard() {
    grid.clear();
    grid.resize(15, std::vector<Tile>(11));
    // Set all to blank
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 11; ++j) {
            Tile blankTile{' ', false, nullptr};
                grid[i][j] = blankTile;
        }
    }
}

