#pragma once

#include "core/SolverEvent.hpp"
#include "core/SolverStatistics.hpp"

#include <vector>

class ISolver
{
public:
    virtual ~ISolver() = default;

    virtual void generateEvents() = 0;

    virtual bool hasMoreEvents() const = 0;

    virtual const SolverEvent&
    getCurrentEvent() const = 0;

    virtual void advanceEvent() = 0;

    virtual void resetPlayback() = 0;

    virtual const std::vector<SolverEvent>&
    getEvents() const = 0;

    virtual const SolverStatistics&
    getStatistics() const = 0;

    virtual const std::vector<std::vector<int>>&
    getSolutions() const = 0;
};