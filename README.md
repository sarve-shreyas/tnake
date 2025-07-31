# TSnake 🐍

A terminal-based Snake game implementation written in C, featuring colorful ANSI graphics and classic Snake gameplay.

## About

TSnake is a console-based Snake game that runs directly in your terminal. The game features:

- **Terminal Graphics**: Uses ANSI escape sequences for colorful terminal output
- **Classic Gameplay**: Traditional Snake mechanics - eat food, grow longer, avoid walls and yourself
- **Modular Design**: Clean C code architecture with separated concerns
- **Cross-Platform**: Runs on macOS, Linux, and Windows terminals that support ANSI colors

## Project Structure

```
tnake/
├── src/                    # Source files
│   ├── main.c             # Main game loop and initialization
│   ├── snake.c            # Snake logic and movement
│   ├── gameboard.c        # Game board management
│   ├── terminal.c         # Terminal utilities and window sizing
│   ├── ansi.c             # ANSI color and style utilities
│   ├── objectspace.c      # Object rendering and space management
│   └── utils.c            # Utility functions
├── include/               # Header files
│   ├── snake.h            # Snake data structures and functions
│   ├── gameboard.h        # Game board definitions
│   ├── terminal.h         # Terminal utilities
│   ├── ansi.h             # ANSI color constants and functions
│   ├── objectspace.h      # Object space management
│   ├── space.h            # Space representation styles
│   └── utils.h            # Utility function declarations
├── CMakeLists.txt         # CMake build configuration
├── .gitignore            # Git ignore rules
└── README.md             # This file
```

## Features

- 🎮 **Classic Snake Gameplay**: Navigate the snake to eat food and grow
- 🌈 **Colorful Graphics**: ANSI terminal colors for enhanced visual experience
- 📏 **Dynamic Board Size**: Adapts to your terminal window size
- 🏗️ **Modular Code**: Well-organized C code with clear separation of concerns
- 🔧 **Build System**: CMake-based build system with Debug/Release configurations
- 🧪 **Memory Safety**: Address sanitizer and debugging tools in Debug mode

## Prerequisites

- **C Compiler**: GCC, Clang, or compatible C99 compiler
- **CMake**: Version 3.10 or higher
- **Terminal**: ANSI color support (most modern terminals)

### Installation on macOS
```bash
# Install Xcode Command Line Tools (includes GCC/Clang)
xcode-select --install

# Install CMake via Homebrew
brew install cmake
```

### Installation on Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake
```

## Building the Project

### Method 1: Using CMake (Recommended)

1. **Clone or download the project**
```bash
git clone <repository-url>
cd tnake
```

2. **Create build directory**
```bash
mkdir build
cd build
```

3. **Configure and build**
```bash
# Debug build (default, includes debugging symbols and sanitizers)
cmake ..
make

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

4. **Run the game**
```bash
./bin/tnake
```

### Method 2: Direct CMake Build

```bash
# Build directly in project root
cmake .
make

# Run the game
./bin/tnake
```

### Available Make Targets

- `make` or `make all` - Build the executable
- `make clean` - Remove build artifacts
- `make run` - Build and run the game
- `make install` - Install the game system-wide

## How to Play

1. **Launch the game**: Run `./bin/tnake` in your terminal
2. **Controls**: Use arrow keys or WASD to control the snake
3. **Objective**: 
   - Guide the snake to eat food (usually represented by special characters)
   - Each food item makes the snake grow longer
   - Avoid hitting the walls or the snake's own body
4. **Scoring**: Your score increases as the snake grows
5. **Game Over**: The game ends when the snake hits a wall or itself



Enjoy playing TSnake! 🐍✨
