#include "core/Board.hpp"

#include <cassert>

void testBoardCreation()
{
    Board board(8);

    assert(board.getSize() == 8);
    assert(board.getQueens().empty());
}

void testPlaceQueen()
{
    Board board(4);

    assert(board.placeQueen(0, 1));
    assert(board.hasQueen(0, 1));

    assert(!board.placeQueen(0, 1));
}

void testInvalidPosition()
{
    Board board(4);

    assert(!board.placeQueen(-1, 0));
    assert(!board.placeQueen(4, 0));
    assert(!board.placeQueen(0, 4));
}

void testRemoveQueen()
{
    Board board(4);

    board.placeQueen(1, 2);

    assert(board.removeQueen(1, 2));
    assert(!board.hasQueen(1, 2));

    assert(!board.removeQueen(1, 2));
}

void testClear()
{
    Board board(4);

    board.placeQueen(0, 0);
    board.placeQueen(1, 2);

    board.clear();

    assert(board.getQueens().empty());
}

int main()
{
    testBoardCreation();
    testPlaceQueen();
    testInvalidPosition();
    testRemoveQueen();
    testClear();

    return 0;
}