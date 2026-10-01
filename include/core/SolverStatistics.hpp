#pragma once

#include <cstddef>

struct SolverStatistics
{
    int solutions = 0;

    long long recursiveCalls = 0;
    long long placements = 0;
    long long conflicts = 0;
    long long backtracks = 0;

    std::size_t generatedEvents = 0;

    double generationTimeMs = 0.0;
};