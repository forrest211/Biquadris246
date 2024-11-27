#include "block.h"
#include <memory>
#include <iostream>

// Abstract Block class
Block::~Block(){} // Destructor does nothing
std::vector<std::pair<int, int>> Block::getCoords() const { return coords; }
char Block::getBlockSymbol() { return tileSymbol; }

// New coords for rotation
// Get rotated coords
std::vector<std::pair<int, int>> Block::computeRotatedCoords(string dir) const {
    std::vector<std::pair<int, int>> res;
    std::pair<int, int> prevBottomLeft = bottomLeft;
    int adjustX;
    int adjustY;
    // CW rotation
    if (dir == "CW") {
        std::pair<int, int> rotatedBottomRight = {-(bottomRight.second), bottomRight.first};
        adjustX = prevBottomLeft.first - rotatedBottomRight.first;
        adjustY = prevBottomLeft.second - rotatedBottomRight.second;
        for (auto it : coords) {
            res.emplace_back(-(it.second) + adjustX, it.first + adjustY);
        }
    }
    // CCW rotation
    else {
        std::pair<int, int> rotatedTopLeft = {topLeft.second, -(topLeft.first)};
        adjustX = prevBottomLeft.first - rotatedTopLeft.first;
        adjustY = prevBottomLeft.second - rotatedTopLeft.second;
        for (auto it : coords) {
            res.emplace_back(it.second + adjustX, -(it.first) + adjustY);
        }
    }
    return res;
}
// Actually rotate the Block
void Block::rotate(string dir) {
    coords = computeRotatedCoords(dir);
    // Update the corners
    std::pair<int, int> prevBottomLeft = bottomLeft;
    int adjustX;
    int adjustY;
    if (dir == "CW") {
        std::pair<int, int> rotatedBottomRight = {-(bottomRight.second), bottomRight.first};
        std::pair<int, int> rotatedTopRight = {-(topRight.second), topRight.first};
        std::pair<int, int> rotatedBottomLeft = {-(bottomLeft.second), bottomLeft.first};
        std::pair<int, int> rotatedTopLeft = {-(topLeft.second), topLeft.first};
        adjustX = prevBottomLeft.first - rotatedBottomRight.first;
        adjustY = prevBottomLeft.second - rotatedBottomRight.second;
        // Mutates here
        bottomLeft = {rotatedBottomRight.first + adjustX, rotatedBottomRight.second + adjustY};
        bottomRight = {rotatedTopRight.first + adjustX, rotatedTopRight.second + adjustY};
        topLeft = {rotatedBottomLeft.first + adjustX, rotatedBottomLeft.second + adjustY};
        topRight = {rotatedTopLeft.first + adjustX, rotatedTopLeft.second + adjustY};
    }
    else {
        std::pair<int, int> rotatedBottomRight = {bottomRight.second, -(bottomRight.first)};
        std::pair<int, int> rotatedTopRight = {topRight.second, -(topRight.first)};
        std::pair<int, int> rotatedBottomLeft = {bottomLeft.second, -(bottomLeft.first)};
        std::pair<int, int> rotatedTopLeft = {topLeft.second, -(topLeft.first)};
        adjustX = prevBottomLeft.first - rotatedTopLeft.first;
        adjustY = prevBottomLeft.second - rotatedTopLeft.second;
        // Mutates here
        bottomLeft = {rotatedTopLeft.first + adjustX, rotatedTopLeft.second + adjustY};
        bottomRight = {rotatedBottomLeft.first + adjustX, rotatedBottomLeft.second + adjustY};
        topLeft = {rotatedTopRight.first + adjustX, rotatedTopRight.second + adjustY};
        topRight = {rotatedBottomRight.first + adjustX, rotatedBottomRight.second + adjustY};
    }
}

// New coords for movement
std::vector<std::pair<int, int>> Block::computeMovedCoords(string dir) const {
    std::vector<std::pair<int, int>> res;
    // Move left
    if (dir == "l") {
        for (auto it : coords) {
            res.emplace_back(it.first - 1, it.second);
        }
    }
    // Move right
    else if (dir == "r") {
        for (auto it : coords) {
            res.emplace_back(it.first + 1, it.second);
        }
    }
    // Move down
    else {
        for (auto it : coords) {
            res.emplace_back(it.first, it.second + 1);
        }
    }
    return res;
}
// Actually move the block
void Block::move(string dir) {
    coords = computeMovedCoords(dir);
    // Update the corners
    // Move left
    if (dir == "l") {
        bottomLeft.first -= 1;
        bottomRight.first -= 1;
        topLeft.first -= 1;
        topRight.first -= 1;
    }
    // Move right
    else if (dir == "r") {
        bottomLeft.first += 1;
        bottomRight.first += 1;
        topLeft.first += 1;
        topRight.first += 1;
    }
    // Move down
    else {
        bottomLeft.second += 1;
        bottomRight.second += 1;
        topLeft.second += 1;
        topRight.second += 1;
    }
}


// Derived classes

// OBlock class

OBlock::OBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'O';
    coords = {{0, 2}, {1, 2}, {0, 3}, {1, 3}}; // Default position on the board when dropped
    bottomLeft = {0,3};
    bottomRight = {1,3};
    topLeft = {0,2};
    topRight = {1,2};
}
// New coords for a OBlock rotation
std::vector<std::pair<int, int>> OBlock::computeRotatedCoords(string dir) const {
    return coords;
}
// Construct and return the tile by value
Tile OBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
OBlock::~OBlock() {
    player->scoreBlock(origLvl);
}
*/


// IBlock class

IBlock::IBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'I';
    coords = {{0, 3}, {1, 3}, {2, 3}, {3, 3}};
    bottomLeft = {0,3};
    bottomRight = {3,3};
    topLeft = {0,3};
    topRight = {3,3};
}
// Construct and return the tile by value
Tile IBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
IBlock::~IBlock() {
    std::cout << "destructor called" << std::endl;
    player->scoreBlock(origLvl);
}


// SBlock class

// Constructor
SBlock::SBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'S';
    coords = {{0, 3}, {1, 3}, {1, 2}, {2, 2}};
    bottomLeft = {0,3};
    bottomRight = {2,3};
    topLeft = {0,2};
    topRight = {2,2};
}
// Construct and return the tile by value
Tile SBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
SBlock::~SBlock() {
    player->scoreBlock(origLvl);
}
*/


// ZBlock class

// Constructor
ZBlock::ZBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'Z';
    coords = {{0, 2}, {1, 2}, {1, 3}, {2, 3}};
    bottomLeft = {0,3};
    bottomRight = {2,3};
    topLeft = {0,2};
    topRight = {2,2};
}
// Construct and return the tile by value
Tile ZBlock::getBlockTile() {
    return Tile(tileSymbol, true, shared_from_this());
}
/*
ZBlock::~ZBlock() {
    player->scoreBlock(origLvl);
}
*/


// JBlock class

// Constructor
JBlock::JBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'J';
    coords = {{0, 2}, {0, 3}, {1, 3}, {2, 3}};
    bottomLeft = {0,3};
    bottomRight = {2,3};
    topLeft = {0,2};
    topRight = {2,2};
}
// Construct and return the tile by value
Tile JBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
JBlock::~JBlock() {
    player->scoreBlock(origLvl);
}
*/


// LBlock class

// Constructor
LBlock::LBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'L';
    coords = {{2, 2}, {0, 3}, {1, 3}, {2, 3}};
    bottomLeft = {0,3};
    bottomRight = {2,3};
    topLeft = {0,2};
    topRight = {2,2};
}
// Construct and return the tile by value
Tile LBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
LBlock::~LBlock() {
    player->scoreBlock(origLvl);
}
*/


// TBlock class

// Constructor
TBlock::TBlock(int lvl, Player *player) {
    origLvl = lvl;
    this->player = player;
    tileSymbol = 'T';
    coords = {{0, 2}, {1, 2}, {2, 2}, {1, 3}};
    bottomLeft = {0,3};
    bottomRight = {2,3};
    topLeft = {0,2};
    topRight = {2,2};
}
// Construct and return the tile by value
Tile TBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
TBlock::~TBlock() {
    player->scoreBlock(origLvl);
}
*/


// Constructor
StarBlock::StarBlock(Player *player) {
    origLvl = 4;
    this->player = player;
    tileSymbol = '*';
    coords = {{5,0}};
    bottomLeft = {5,0};
    bottomRight = {5,0};
    topLeft = {5,0};
    topRight = {5,0};
}
// Construct and return the tile by value
Tile StarBlock::getBlockTile() { 
    return Tile(tileSymbol, true, shared_from_this());
}
/*
StarBlock::~StarBlock() {
    player->scoreBlock(origLvl);
}
*/

