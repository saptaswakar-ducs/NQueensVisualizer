#include "core/Board.hpp"

Board::Board(int size)
    : size(size)
{
}

int Board::getSize() const
{
    return size;
}

bool Board::isValidPosition(int row, int column) const
{
    return row >= 0 &&
           row < size &&
           column >= 0 &&
           column < size;
}

bool Board::placeQueen(int row, int column)
{
    if (!isValidPosition(row, column))
    {
        return false;
    }

    if (hasQueen(row, column))
    {
        return false;
    }

    queens.emplace_back(row, column);

    return true;
}

bool Board::removeQueen(int row, int column)
{
    for (auto it = queens.begin(); it != queens.end(); ++it)
    {
        if (it->getRow() == row &&
            it->getColumn() == column)
        {
            queens.erase(it);
            return true;
        }
    }

    return false;
}

bool Board::hasQueen(int row, int column) const
{
    for (const Queen& queen : queens)
    {
        if (queen.getRow() == row &&
            queen.getColumn() == column)
        {
            return true;
        }
    }

    return false;
}

const std::vector<Queen>& Board::getQueens() const
{
    return queens;
}

void Board::clear()
{
    queens.clear();
}