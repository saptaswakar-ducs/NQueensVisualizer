#include "core/Queen.hpp"

Queen::Queen(int row, int column)
    : row(row), column(column)
{
}

int Queen::getRow() const
{
    return row;
}

int Queen::getColumn() const
{
    return column;
}