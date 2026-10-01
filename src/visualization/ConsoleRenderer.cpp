#include "visualization/ConsoleRenderer.hpp"

#include <iostream>

void ConsoleRenderer::render(
    const Board& board,
    const SolverEvent* event
) const
{
    const int size = board.getSize();

    // Clear terminal and move cursor to top-left.
    std::cout << "\033[2J\033[H";

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "          N-QUEENS VISUALIZER\n";
    std::cout << "========================================\n\n";

    renderHeader(size);
    renderColumnLabels(size);

    for (int row = 0; row < size; ++row)
    {
        renderRow(board, row, event);
    }

    renderHeader(size);

    renderEventInfo(event);

    std::cout << "\n";
}

void ConsoleRenderer::renderHeader(int size) const
{
    std::cout << "    ";

    for (int column = 0; column < size; ++column)
    {
        std::cout << "+---";
    }

    std::cout << "+\n";
}

void ConsoleRenderer::renderColumnLabels(int size) const
{
    std::cout << "    ";

    for (int column = 0; column < size; ++column)
    {
        std::cout << " " << column << "  ";
    }

    std::cout << "\n";
}

void ConsoleRenderer::renderRow(
    const Board& board,
    int row,
    const SolverEvent* event
) const
{
    const int size = board.getSize();

    std::cout << " " << row << "  ";

    for (int column = 0; column < size; ++column)
    {
        std::cout << "| ";

        if (board.hasQueen(row, column))
        {
            std::cout << "Q";
        }
        else if (
            event != nullptr &&
            event->row == row &&
            event->column == column &&
            event->type == SolverEventType::Conflict
        )
        {
            std::cout << "X";
        }
        else if (
            event != nullptr &&
            event->row == row &&
            event->column == column &&
            event->type == SolverEventType::TryPosition
        )
        {
            std::cout << "?";
        }
        else
        {
            std::cout << ".";
        }

        std::cout << " ";
    }

    std::cout << "|\n";

    std::cout << "    ";

    for (int column = 0; column < size; ++column)
    {
        std::cout << "+---";
    }

    std::cout << "+\n";
}

void ConsoleRenderer::renderEventInfo(
    const SolverEvent* event
) const
{
    if (event == nullptr)
    {
        std::cout << "Status: Ready\n";
        return;
    }

    std::cout << "\n";

    switch (event->type)
    {
        case SolverEventType::TryPosition:
            std::cout
                << "Status: Trying position ("
                << event->row
                << ", "
                << event->column
                << ")\n";
            break;

        case SolverEventType::PlaceQueen:
            std::cout
                << "Status: Queen placed at ("
                << event->row
                << ", "
                << event->column
                << ")\n";
            break;

        case SolverEventType::Conflict:
            std::cout
                << "Status: Conflict at ("
                << event->row
                << ", "
                << event->column
                << ")\n";
            break;

        case SolverEventType::RemoveQueen:
            std::cout
                << "Status: Backtracking - removed queen from ("
                << event->row
                << ", "
                << event->column
                << ")\n";
            break;

        case SolverEventType::SolutionFound:

            std::cout
                << "Status: *** SOLUTION "
                << event->solutionNumber
                << " FOUND ***\n";

            break;

        case SolverEventType::Finished:

            std::cout
                << "Status: Search completed.\n";

            break;
    }
}