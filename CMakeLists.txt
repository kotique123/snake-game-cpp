cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define platform-specific compile definitions
if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
    message(STATUS "Building for Windows")
elseif(UNIX)
    add_definitions(-DUNIX_PLATFORM)
    message(STATUS "Building for Unix/Linux")
endif()

# Create the executable
add_executable(snake-game src/snake.cpp)

# Link platform-specific libraries
if(WIN32)
    # No additional libraries needed for Windows console API
    target_compile_definitions(snake-game PRIVATE WINDOWS_PLATFORM)
elseif(UNIX)
    # No additional libraries needed for Unix terminal functions
    target_compile_definitions(snake-game PRIVATE UNIX_PLATFORM)
endif()

# Optional: Set different compiler flags for different platforms
if(WIN32)
    # Windows-specific compiler flags
    target_compile_options(snake-game PRIVATE /W4)
elseif(UNIX)
    # Unix-specific compiler flags
    target_compile_options(snake-game PRIVATE -Wall -Wextra)
endif()

# Optional: Create platform-specific install rules
install(TARGETS snake-game DESTINATION bin)

# Optional: Add a custom target for running the game
add_custom_target(run
    COMMAND snake_game
    DEPENDS snake_game
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)