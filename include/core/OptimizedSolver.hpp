#pragma once

#include "core/Board.hpp"
#include "core/ISolver.hpp"

#include <vector>

class OptimizedSolver : public ISolver
{
public:
    explicit OptimizedSolver(Board& board);

    void generateEvents() override;

    bool hasMoreEvents() const override;

    const SolverEvent&
    getCurrentEvent() const override;

    void advanceEvent() override;

    void resetPlayback() override;

    const std::vector<SolverEvent>&
    getEvents() const override;

    const SolverStatistics&
    getStatistics() const override;

    const std::vector<std::vector<int>>&
    getSolutions() const override;

private:
    void backtrack(int row);

    bool isSafe(
        int row,
        int column
    ) const;

    void emitEvent(
        SolverEventType type,
        int row = -1,
        int column = -1,
        int solutionNumber = 0
    );

private:
    Board& board;

    std::vector<bool> columns;
    std::vector<bool> diagonals;
    std::vector<bool> antiDiagonals;

    SolverStatistics statistics;

    std::vector<std::vector<int>> solutions;

    std::vector<SolverEvent> events;

    std::size_t currentEventIndex;
};