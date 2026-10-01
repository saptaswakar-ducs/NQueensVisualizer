#pragma once

enum class SolverEventType
{
    TryPosition,
    PlaceQueen,
    Conflict,
    RemoveQueen,
    SolutionFound,
    Finished
};

struct SolverEvent
{
    SolverEventType type;

    int row;
    int column;

    int solutionNumber;

    SolverEvent(
        SolverEventType type,
        int row = -1,
        int column = -1,
        int solutionNumber = 0
    )
        : type(type),
          row(row),
          column(column),
          solutionNumber(solutionNumber)
    {
    }
};