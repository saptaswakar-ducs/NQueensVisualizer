#pragma once

#include "core/Board.hpp"
#include "core/SolverEvent.hpp"

class ConsoleRenderer
{
public:
    void render(
        const Board& board,
        const SolverEvent* event = nullptr
    ) const;

private:
    void renderHeader(int size) const;

    void renderColumnLabels(int size) const;

    void renderRow(
        const Board& board,
        int row,
        const SolverEvent* event
    ) const;

    void renderEventInfo(
        const SolverEvent* event
    ) const;
};