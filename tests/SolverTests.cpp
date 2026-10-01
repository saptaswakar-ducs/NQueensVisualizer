#include "core/Board.hpp"
#include "core/Solver.hpp"
#include "core/OptimizedSolver.hpp"
#include "core/BitmaskSolver.hpp"

#include <cassert>

void testStandardSolver()
{
    Board board(4);

    Solver solver(board);

    solver.generateEvents();

    assert(
        solver.getStatistics().solutions == 2
    );
}

void testOptimizedSolver()
{
    Board board(4);

    OptimizedSolver solver(board);

    solver.generateEvents();

    assert(
        solver.getStatistics().solutions == 2
    );
}

void testBitmaskSolver()
{
    Board board(4);

    BitmaskSolver solver(board);

    solver.generateEvents();

    assert(
        solver.getStatistics().solutions == 2
    );
}

void testEightQueens()
{
    Board board1(8);
    Board board2(8);
    Board board3(8);

    Solver standard(board1);
    OptimizedSolver optimized(board2);
    BitmaskSolver bitmask(board3);

    standard.generateEvents();
    optimized.generateEvents();
    bitmask.generateEvents();

    assert(
        standard.getStatistics().solutions == 92
    );

    assert(
        optimized.getStatistics().solutions == 92
    );

    assert(
        bitmask.getStatistics().solutions == 92
    );
}

int main()
{
    testStandardSolver();
    testOptimizedSolver();
    testBitmaskSolver();
    testEightQueens();

    return 0;
}