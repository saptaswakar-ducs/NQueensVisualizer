#pragma once

class Queen
{
public:
    Queen(int row, int column);

    int getRow() const;
    int getColumn() const;

private:
    int row;
    int column;
};