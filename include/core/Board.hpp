#pragma once

#include "core/Queen.hpp"

#include <vector>

class Board
{
public:
    explicit Board(int size);

    int getSize() const;

    bool placeQueen(int row, int column);
    bool removeQueen(int row, int column);

    bool hasQueen(int row, int column) const;

    const std::vector<Queen>& getQueens() const;

    void clear();

private:
    bool isValidPosition(int row, int column) const;

private:
    int size;
    std::vector<Queen> queens;
};