#pragma once

#include "ApplicationState.hpp"
#include "PlaybackMode.hpp"
#include "core/AlgorithmType.hpp"
#include "core/ISolver.hpp"

#include <memory>

#include "core/Board.hpp"
#include "core/Solver.hpp"
#include "core/SolverEvent.hpp"

#include "visualization/ConsoleRenderer.hpp"

#include <cstddef>

class Application
{
public:
    Application();

    void run();

private:
    void showWelcomeScreen();

    void setup();

    void runAutomaticMode();

    void runStepMode();

    void processNextEvent();

    void applyEventToBoard(
        const SolverEvent& event
    );

    void render();

    void displayStatistics() const;

    void displayProgress() const;

    void resetPlayback();

    void displayAlgorithm() const;

    void showHelp();
    void compareAlgorithms(int size);

private:
    int boardSize;
    int animationDelay;

    Board board;
    std::unique_ptr<ISolver> solver;
    

    ConsoleRenderer renderer;

    ApplicationState state;
    PlaybackMode playbackMode;
    AlgorithmType algorithmType;

    SolverEvent currentEvent;

    std::size_t processedEvents;
};