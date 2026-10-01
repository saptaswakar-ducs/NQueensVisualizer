#include "core/BitmaskSolver.hpp"

#include <chrono>

BitmaskSolver::BitmaskSolver(
    Board& board
)
    : board(board),
      currentEventIndex(0),
      boardSize(board.getSize())
{
}

void BitmaskSolver::generateEvents()
{
    board.clear();

    boardSize = board.getSize();

    statistics = SolverStatistics{};

    solutions.clear();
    events.clear();

    currentEventIndex = 0;

    if (boardSize > 63)
    {
        return;
    }

    const auto start =
        std::chrono::high_resolution_clock::now();

    backtrack(
        0,
        0,
        0,
        0
    );

    emitEvent(
        SolverEventType::Finished
    );

    const auto end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli>
        duration = end - start;

    statistics.generationTimeMs =
        duration.count();

    statistics.generatedEvents =
        events.size();

    board.clear();
}

void BitmaskSolver::backtrack(
    int row,
    std::uint64_t columns,
    std::uint64_t diagonals,
    std::uint64_t antiDiagonals
)
{
    ++statistics.recursiveCalls;

    if (row == boardSize)
    {
        ++statistics.solutions;

        std::vector<int> solution;

        for (const Queen& queen :
             board.getQueens())
        {
            solution.push_back(
                queen.getColumn()
            );
        }

        solutions.push_back(solution);

        emitEvent(
            SolverEventType::SolutionFound,
            -1,
            -1,
            statistics.solutions
        );

        return;
    }

    const std::uint64_t allColumns =
        (1ULL << boardSize) - 1;

    const std::uint64_t occupied =
        columns |
        diagonals |
        antiDiagonals;

    const std::uint64_t available =
        allColumns & ~occupied;

    std::uint64_t positions =
        available;

    while (positions != 0)
    {
        const std::uint64_t bit =
            positions &
            (~positions + 1);

        positions &= positions - 1;

        int column = 0;

        std::uint64_t temp = bit;

        while (temp > 1)
        {
            temp >>= 1;
            ++column;
        }

        emitEvent(
            SolverEventType::TryPosition,
            row,
            column
        );

        board.placeQueen(
            row,
            column
        );

        ++statistics.placements;

        emitEvent(
            SolverEventType::PlaceQueen,
            row,
            column
        );

        backtrack(
            row + 1,
            columns | bit,
            (diagonals | bit) << 1,
            (antiDiagonals | bit) >> 1
        );

        board.removeQueen(
            row,
            column
        );

        ++statistics.backtracks;

        emitEvent(
            SolverEventType::RemoveQueen,
            row,
            column
        );
    }
}

void BitmaskSolver::emitEvent(
    SolverEventType type,
    int row,
    int column,
    int solutionNumber
)
{
    events.emplace_back(
        type,
        row,
        column,
        solutionNumber
    );
}

bool BitmaskSolver::hasMoreEvents() const
{
    return currentEventIndex <
           events.size();
}

const SolverEvent&
BitmaskSolver::getCurrentEvent() const
{
    return events.at(
        currentEventIndex
    );
}

void BitmaskSolver::advanceEvent()
{
    if (currentEventIndex <
        events.size())
    {
        ++currentEventIndex;
    }
}

void BitmaskSolver::resetPlayback()
{
    currentEventIndex = 0;
}

const std::vector<SolverEvent>&
BitmaskSolver::getEvents() const
{
    return events;
}

const SolverStatistics&
BitmaskSolver::getStatistics() const
{
    return statistics;
}

const std::vector<std::vector<int>>&
BitmaskSolver::getSolutions() const
{
    return solutions;
}