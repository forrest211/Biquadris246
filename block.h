#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "tile.h"
#include "player.h"

using namespace std;

class Block : public std::enable_shared_from_this<Block>{
  protected:
    // Char that the Tiles will be made of
    char tileSymbol;
    // Level that the Block originated from
    int origLvl;
    // Pointer to the player so that it can update the score
    Player *player;
    // Vector of pairs to store the relative coords of the Tiles forming the Block
    std::vector<std::pair<int, int>> coords;
    // Keep track of the following corners to adjust the rotations
    std::pair<int, int> bottomLeft;
    std::pair<int, int> bottomRight;
    std::pair<int, int> topLeft;
    std::pair<int, int> topRight;
  public:
    friend class board;
    std::vector<std::pair<int, int>> getCoords() const;
    virtual ~Block() = 0;
    virtual Tile getBlockTile() = 0; // THIS SHOULD ONLY BE CALLED WHEN PLACING A TILE ON THE BOARD
    char getBlockSymbol();
    
    // Get the new coords when rotating (give "CW" or "CCW")
    virtual std::vector<std::pair<int, int>> computeRotatedCoords(string dir) const;
    // Actually rotate the block
    void rotate(string dir);

    // Get the new coords when moving (give "l", "r", or "d")
    virtual std::vector<std::pair<int, int>> computeMovedCoords(string dir) const;
    // Actually move the block
    void move(string dir);
};

// OBlock
class OBlock : public Block {
  public:
    OBlock(int origLvl, Player *player);
    // Override because rotation on OBlock does nothing
    std::vector<std::pair<int, int>> computeRotatedCoords(string dir) const override;
    Tile getBlockTile() override;
    ~OBlock() override;
};

// IBlock
class IBlock : public Block {
  public:
    IBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~IBlock() override;
};

// SBlock
class SBlock : public Block {
  public:
    SBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~SBlock() override;
};

// ZBlock
class ZBlock : public Block {
  public:
    ZBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~ZBlock() override;
};

// JBlock
class JBlock : public Block {
  public:
    JBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~JBlock() override;
};

// LBlock
class LBlock : public Block {
  public:
    LBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~LBlock() override;
};

// TBlock
class TBlock : public Block {
  public:
    TBlock(int origLvl, Player *player);
    Tile getBlockTile() override;
    ~TBlock() override;
};

// StarBlock
class StarBlock : public Block {
  public:
    StarBlock(Player *player);
    Tile getBlockTile() override;
    ~StarBlock() override;
};

#endif
