#include "core/OptimizedSolver.hpp"

#include <chrono>

OptimizedSolver::OptimizedSolver(Board& board)
    : board(board),
      currentEventIndex(0)
{
}

void OptimizedSolver::generateEvents()
{
    const int n = board.getSize();

    board.clear();

    columns.assign(n, false);
    diagonals.assign(
        2 * n - 1,
        false
    );

    antiDiagonals.assign(
        2 * n - 1,
        false
    );

    statistics = SolverStatistics{};

    solutions.clear();
    events.clear();

    currentEventIndex = 0;

    const auto start =
        std::chrono::high_resolution_clock::now();

    backtrack(0);

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

bool OptimizedSolver::isSafe(
    int row,
    int column
) const
{
    const int diagonal =
        row - column +
        board.getSize() - 1;

    const int antiDiagonal =
        row + column;

    return
        !columns[column] &&
        !diagonals[diagonal] &&
        !antiDiagonals[antiDiagonal];
}

void OptimizedSolver::backtrack(int row)
{
    ++statistics.recursiveCalls;

    const int n = board.getSize();

    if (row == n)
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
         column < n;
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

        columns[column] = true;

        const int diagonal =
            row - column + n - 1;

        const int antiDiagonal =
            row + column;

        diagonals[diagonal] = true;
        antiDiagonals[antiDiagonal] = true;

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

        columns[column] = false;
        diagonals[diagonal] = false;
        antiDiagonals[antiDiagonal] = false;

        ++statistics.backtracks;

        emitEvent(
            SolverEventType::RemoveQueen,
            row,
            column
        );
    }
}

void OptimizedSolver::emitEvent(
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

bool OptimizedSolver::hasMoreEvents() const
{
    return currentEventIndex <
           events.size();
}

const SolverEvent&
OptimizedSolver::getCurrentEvent() const
{
    return events.at(
        currentEventIndex
    );
}

void OptimizedSolver::advanceEvent()
{
    if (currentEventIndex <
        events.size())
    {
        ++currentEventIndex;
    }
}

void OptimizedSolver::resetPlayback()
{
    currentEventIndex = 0;
}

const std::vector<SolverEvent>&
OptimizedSolver::getEvents() const
{
    return events;
}

const SolverStatistics&
OptimizedSolver::getStatistics() const
{
    return statistics;
}

const std::vector<std::vector<int>>&
OptimizedSolver::getSolutions() const
{
    return solutions;
}