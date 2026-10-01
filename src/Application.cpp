#include "Application.hpp"
#include "core/OptimizedSolver.hpp"
#include "core/BitmaskSolver.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>

Application::Application()
    : boardSize(4),
      animationDelay(150),
      board(4),
      solver(nullptr),
      state(ApplicationState::Ready),
      playbackMode(PlaybackMode::Automatic),
      algorithmType(
          AlgorithmType::StandardBacktracking
      ),
      currentEvent(
          SolverEventType::Finished
      ),
      processedEvents(0)
{
}

void Application::run()
{
    showWelcomeScreen();

    setup();

    if (playbackMode ==
        PlaybackMode::Automatic)
    {
        runAutomaticMode();
    }
    else
    {
        runStepMode();
    }

    displayStatistics();
}

void Application::showWelcomeScreen()
{
    std::cout << "\n";
    std::cout
        << "========================================\n";
    std::cout
        << "        N-QUEENS VISUALIZER\n";
    std::cout
        << "========================================\n\n";

    std::cout
        << "Phase 6 - Statistics & Performance\n\n";
}

void Application::setup()
{
    std::cout << "Enter board size N: ";
    std::cin >> boardSize;

    if (boardSize < 1)
    {
        std::cout
            << "Invalid board size. Using N = 4.\n";

        boardSize = 4;
    }

    board = Board(boardSize);

    std::cout << "\n";
    std::cout << "Select algorithm:\n";
    std::cout << "1. Standard Backtracking\n";
    std::cout << "2. Optimized Backtracking\n";
    std::cout << "3. Bitmask\n";
    std::cout << "Choice: ";

    int algorithmChoice;
    std::cin >> algorithmChoice;

    switch (algorithmType)
    {
        case AlgorithmType::StandardBacktracking:

            solver =
                std::make_unique<Solver>(board);

            break;

        case AlgorithmType::OptimizedBacktracking:

            solver =
                std::make_unique<OptimizedSolver>(
                    board
                );

            break;

        case AlgorithmType::Bitmask:

            solver =
                std::make_unique<BitmaskSolver>(
                    board
                );

            break;
    }
    if (
        algorithmType == AlgorithmType::Bitmask &&
        boardSize > 63
    )
    {
        std::cout
            << "\nBitmask solver supports N <= 63.\n"
            << "Falling back to optimized backtracking.\n";

        algorithmType =
            AlgorithmType::OptimizedBacktracking;
    }

    std::cout << "\n";
    std::cout << "Select playback mode:\n";
    std::cout << "1. Automatic\n";
    std::cout << "2. Step-by-step\n";
    std::cout << "Choice: ";

    int modeChoice;
    std::cin >> modeChoice;

    if (modeChoice == 2)
    {
        playbackMode =
            PlaybackMode::StepByStep;
    }
    else
    {
        playbackMode =
            PlaybackMode::Automatic;
    }

    if (playbackMode ==
        PlaybackMode::Automatic)
    {
        std::cout << "\n";
        std::cout << "Select speed:\n";
        std::cout << "1. Slow\n";
        std::cout << "2. Medium\n";
        std::cout << "3. Fast\n";
        std::cout << "Choice: ";

        int speedChoice;
        std::cin >> speedChoice;

        switch (speedChoice)
        {
            case 1:
                animationDelay = 500;
                break;

            case 2:
                animationDelay = 150;
                break;

            case 3:
                animationDelay = 30;
                break;

            default:
                animationDelay = 150;
        }
    }

    std::cout << "\nGenerating solution events...\n";

    solver->generateEvents();

    resetPlayback();

    state = ApplicationState::Ready;
}

void Application::runAutomaticMode()
{
    state = ApplicationState::Running;

    while (solver->hasMoreEvents())
    {
        processNextEvent();

        render();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                animationDelay
            )
        );
    }

    state = ApplicationState::Finished;
}

void Application::runStepMode()
{
    state = ApplicationState::Running;

    // Remove newline left by std::cin >>.
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    while (solver->hasMoreEvents())
    {
        render();

        std::cout << "\n";
        std::cout
            << "Press ENTER for next event "
            << "or Q + ENTER to quit: ";

        std::string command;

        std::getline(
            std::cin,
            command
        );

        if (
            command == "q" ||
            command == "Q"
        )
        {
            break;
        }

        processNextEvent();
    }

    state = ApplicationState::Finished;

    render();
}

void Application::processNextEvent()
{
    if (!solver->hasMoreEvents())
    {
        state =
            ApplicationState::Finished;

        return;
    }

    currentEvent =
        solver->getCurrentEvent();

    applyEventToBoard(
        currentEvent
    );

    solver->advanceEvent();

    ++processedEvents;

    if (
        currentEvent.type ==
        SolverEventType::Finished
    )
    {
        state =
            ApplicationState::Finished;
    }
}

void Application::applyEventToBoard(
    const SolverEvent& event
)
{
    switch (event.type)
    {
        case SolverEventType::PlaceQueen:

            board.placeQueen(
                event.row,
                event.column
            );

            break;

        case SolverEventType::RemoveQueen:

            board.removeQueen(
                event.row,
                event.column
            );

            break;

        default:
            break;
    }
}

void Application::render()
{
    renderer.render(
        board,
        &currentEvent
    );

    displayProgress();
}

void Application::displayProgress() const
{
    const std::size_t totalEvents =
        solver->getEvents().size();

    double progress = 0.0;

    if (totalEvents > 0)
    {
        progress =
            (
                static_cast<double>(
                    processedEvents
                ) /
                static_cast<double>(
                    totalEvents
                )
            ) * 100.0;
    }

    std::cout << "\n";
    std::cout << "Playback\n";
    std::cout << "----------------------------------------\n";

    std::cout
        << "Processed Events : "
        << processedEvents
        << " / "
        << totalEvents
        << "\n";

    std::cout
        << "Progress         : "
        << std::fixed
        << std::setprecision(1)
        << progress
        << "%\n";

    std::cout << "Mode             : ";

    if (
        playbackMode ==
        PlaybackMode::Automatic
    )
    {
        std::cout << "Automatic\n";
    }
    else
    {
        std::cout << "Step-by-step\n";
    }
}

void Application::displayAlgorithm() const
{
    std::cout << "Algorithm           : ";

    switch (algorithmType)
    {
        case AlgorithmType::StandardBacktracking:
            std::cout
                << "Standard Backtracking";
            break;

        case AlgorithmType::OptimizedBacktracking:
            std::cout
                << "Optimized Backtracking";
            break;

        case AlgorithmType::Bitmask:
            std::cout
                << "Bitmask";
            break;
    }

    std::cout << "\n";
}

void Application::displayStatistics() const
{
    const SolverStatistics& stats =
        solver->getStatistics();

    std::cout << "\n";
    std::cout
        << "========================================\n";
    std::cout
        << "             FINAL STATISTICS\n";
    std::cout
        << "========================================\n";

    std::cout
        << "Board Size          : "
        << boardSize
        << "\n";

    std::cout
        << "Solutions           : "
        << stats.solutions
        << "\n";

    std::cout
        << "Recursive Calls     : "
        << stats.recursiveCalls
        << "\n";

    std::cout
        << "Queen Placements    : "
        << stats.placements
        << "\n";

    std::cout
        << "Conflicts           : "
        << stats.conflicts
        << "\n";

    std::cout
        << "Backtracks          : "
        << stats.backtracks
        << "\n";

    std::cout
        << "Generated Events    : "
        << stats.generatedEvents
        << "\n";

    std::cout
        << "Processed Events    : "
        << processedEvents
        << "\n";

    std::cout
        << "Generation Time     : "
        << std::fixed
        << std::setprecision(3)
        << stats.generationTimeMs
        << " ms\n";

    std::cout
        << "========================================\n";
}

void Application::resetPlayback()
{
    board.clear();

    solver->resetPlayback();

    processedEvents = 0;

    currentEvent =
        SolverEvent(
            SolverEventType::Finished
        );
}

void Application::showHelp()
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "                HELP\n";
    std::cout << "========================================\n\n";

    std::cout << "N-Queens Problem\n";
    std::cout << "----------------\n";
    std::cout << "Place N queens on an N x N chessboard so\n";
    std::cout << "that no two queens attack each other.\n\n";

    std::cout << "Algorithms\n";
    std::cout << "----------\n";
    std::cout << "1. Standard Backtracking\n";
    std::cout << "   Checks columns and diagonals directly.\n\n";

    std::cout << "2. Optimized Backtracking\n";
    std::cout << "   Uses occupancy arrays for O(1) safety checks.\n\n";

    std::cout << "3. Bitmask\n";
    std::cout << "   Uses 64-bit bit operations for fast search.\n\n";

    std::cout << "Playback\n";
    std::cout << "--------\n";
    std::cout << "Automatic    : Events play automatically.\n";
    std::cout << "Step-by-step : Press ENTER to advance.\n";
    std::cout << "               Press Q to quit playback.\n\n";

    std::cout << "Controls\n";
    std::cout << "--------\n";
    std::cout << "ENTER : Continue / advance\n";
    std::cout << "Q     : Quit current playback\n\n";
}

void Application::compareAlgorithms(int size)
{
    Board standardBoard(size);
    Board optimizedBoard(size);
    Board bitmaskBoard(size);

    Solver standardSolver(standardBoard);
    OptimizedSolver optimizedSolver(optimizedBoard);
    BitmaskSolver bitmaskSolver(bitmaskBoard);

    standardSolver.generateEvents();
    optimizedSolver.generateEvents();
    bitmaskSolver.generateEvents();

    const SolverStatistics& standardStats =
        standardSolver.getStatistics();

    const SolverStatistics& optimizedStats =
        optimizedSolver.getStatistics();

    const SolverStatistics& bitmaskStats =
        bitmaskSolver.getStatistics();

    std::cout << "\n";
    std::cout << "========================================================\n";
    std::cout << "                 ALGORITHM COMPARISON\n";
    std::cout << "========================================================\n\n";

    std::cout << "Board Size: " << size << "\n\n";

    std::cout
        << "Algorithm"
        << "                 Solutions"
        << "    Recursive Calls"
        << "    Time(ms)\n";

    std::cout
        << "--------------------------------------------------------\n";

    std::cout
        << "Standard Backtracking"
        << "     "
        << standardStats.solutions
        << "          "
        << standardStats.recursiveCalls
        << "          "
        << standardStats.generationTimeMs
        << "\n";

    std::cout
        << "Optimized Backtracking"
        << "    "
        << optimizedStats.solutions
        << "          "
        << optimizedStats.recursiveCalls
        << "          "
        << optimizedStats.generationTimeMs
        << "\n";

    std::cout
        << "Bitmask"
        << "                    "
        << bitmaskStats.solutions
        << "          "
        << bitmaskStats.recursiveCalls
        << "          "
        << bitmaskStats.generationTimeMs
        << "\n";

    std::cout
        << "--------------------------------------------------------\n";
}