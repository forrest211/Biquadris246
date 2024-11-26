#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "tile.h"

using namespace std;

class Block : public std::enable_shared_from_this<Block>{
  protected:
    // Char that the Tiles will be made of
    char tileSymbol;
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
    virtual ~Block() = 0; // To make class abstract
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
    OBlock();
    ~OBlock() override = default;
    // Override because rotation on OBlock does nothing
    std::vector<std::pair<int, int>> computeRotatedCoords(string dir) const override;
    Tile getBlockTile() override;
};

// IBlock
class IBlock : public Block {
  public:
    IBlock();
    ~IBlock() override = default;
    Tile getBlockTile() override;
};

// SBlock
class SBlock : public Block {
  public:
    SBlock();
    ~SBlock() override = default;
    Tile getBlockTile() override;
};

// ZBlock
class ZBlock : public Block {
  public:
    ZBlock();
    ~ZBlock() override = default;
    Tile getBlockTile() override;
};

// JBlock
class JBlock : public Block {
  public:
    JBlock();
    ~JBlock() override = default;
    Tile getBlockTile() override;
};

// LBlock
class LBlock : public Block {
  public:
    LBlock();
    ~LBlock() override = default;
    Tile getBlockTile() override;
};

// TBlock
class TBlock : public Block {
  public:
    TBlock();
    ~TBlock() override = default;
    Tile getBlockTile() override;
};

// StarBlock
class StarBlock : public Block {
  public:
    StarBlock();
    ~StarBlock() override = default;
    Tile getBlockTile() override;
};

#endif
