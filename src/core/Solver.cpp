#include "core/Solver.hpp"

#include <chrono>
#include <cmath>

Solver::Solver(Board& board)
    : board(board),
      currentEventIndex(0)
{
}

void Solver::generateEvents()
{
    board.clear();

    statistics = SolverStatistics{};

    solutions.clear();
    events.clear();

    currentEventIndex = 0;

    const auto start =
        std::chrono::high_resolution_clock::now();

    backtrack(0);

    emitEvent(SolverEventType::Finished);

    const auto end =
        std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli>
        duration = end - start;

    statistics.generationTimeMs =
        duration.count();

    statistics.generatedEvents =
        events.size();

    // Playback must begin with an empty board.
    board.clear();
}

bool Solver::isSafe(int row, int column) const
{
    for (const Queen& queen : board.getQueens())
    {
        const int queenRow =
            queen.getRow();

        const int queenColumn =
            queen.getColumn();

        // Column conflict
        if (queenColumn == column)
        {
            return false;
        }

        // Diagonal conflict
        if (
            std::abs(queenRow - row) ==
            std::abs(queenColumn - column)
        )
        {
            return false;
        }
    }

    return true;
}

void Solver::backtrack(int row)
{
    ++statistics.recursiveCalls;

    const int size = board.getSize();

    if (row == size)
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

    for (int column = 0;
         column < size;
         ++column)
    {
        emitEvent(
            SolverEventType::TryPosition,
            row,
            column
        );

        if (!isSafe(row, column))
        {
            ++statistics.conflicts;

            emitEvent(
                SolverEventType::Conflict,
                row,
                column
            );

            continue;
        }

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

        backtrack(row + 1);

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

void Solver::emitEvent(
    SolverEventType type,
    int row,
    int column,
    int solutionNumber)
{
    events.emplace_back(
        type,
        row,
        column,
        solutionNumber
    );
}

bool Solver::hasMoreEvents() const
{
    return currentEventIndex <
           events.size();
}

const SolverEvent&
Solver::getCurrentEvent() const
{
    return events.at(
        currentEventIndex
    );
}

void Solver::advanceEvent()
{
    if (currentEventIndex <
        events.size())
    {
        ++currentEventIndex;
    }
}

void Solver::resetPlayback()
{
    currentEventIndex = 0;
}

const std::vector<SolverEvent>&
Solver::getEvents() const
{
    return events;
}

const std::vector<std::vector<int>>&
Solver::getSolutions() const
{
    return solutions;
}

const SolverStatistics&
Solver::getStatistics() const
{
    return statistics;
}