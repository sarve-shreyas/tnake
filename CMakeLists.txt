# CMakeLists.txt for C program

# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(tnake VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Debug and Release configurations
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Build type not specified, defaulting to Debug")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Source files
set(SOURCES
    src/main.c
    src/ansi.c
    src/terminal.c
    src/gameboard.c
    src/snake.c
    src/objectspace.c
    src/utils.c
    src/abuffer.c
    src/styles.c
)

# Header files (for IDE support)
set(HEADERS
    include/ansi.h
    include/terminal.h
    include/gameboard.h
    include/snake.h
    include/objectspace.h
    include/space.h
    include/utils.h
    include/translate.h
    include/abuffer.h
    include/styles.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add debug-specific compiler flags for better debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -ggdb3          # Enhanced debugging info for GDB
        -fno-omit-frame-pointer  # Keep frame pointers for better stack traces
        -fsanitize=address       # Address sanitizer (optional, comment out if not needed)
        -fsanitize=undefined     # Undefined behavior sanitizer (optional)
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address
        -fsanitize=undefined
    )
    message(STATUS "Debug mode: Enhanced debugging and sanitizers enabled")
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable testing (optional)
enable_testing()

# Custom target to run the program
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)

# Install rules (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
