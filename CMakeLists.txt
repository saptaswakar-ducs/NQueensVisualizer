cmake_minimum_required(VERSION 3.16)

project(NQueensVisualizer
    VERSION 1.0
    DESCRIPTION "Terminal-based N-Queens Visualizer and Algorithm Comparison Tool"
    LANGUAGES CXX
)

# ============================================================
# C++ Standard
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================
# Source Files
# ============================================================

set(SOURCES

    # Application
    src/main.cpp
    src/Application.cpp

    # Core
    src/core/Board.cpp
    src/core/Queen.cpp
    src/core/Solver.cpp
    src/core/OptimizedSolver.cpp
    src/core/BitmaskSolver.cpp

    # Visualization
    src/visualization/ConsoleRenderer.cpp
)


# ============================================================
# Main Executable
# ============================================================

add_executable(NQueensVisualizer
    ${SOURCES}
)


# ============================================================
# Include Directories
# ============================================================

target_include_directories(NQueensVisualizer
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)


# ============================================================
# Compiler Warnings
# ============================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(NQueensVisualizer
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
elseif(MSVC)
    target_compile_options(NQueensVisualizer
        PRIVATE
            /W4
    )
endif()


# ============================================================
# Testing
# ============================================================

# enable_testing()

# add_subdirectory(tests)