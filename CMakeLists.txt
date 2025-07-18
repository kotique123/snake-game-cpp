cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-DWINDOWS_PLATFORM)
    message(STATUS "Building for Windows")
elseif(UNIX)
    add_definitions(-DUNIX_PLATFORM)
    message(STATUS "Building for Unix/Linux")
endif()

add_executable(snake-game src/snake.cpp)

if(WIN32)
    target_compile_definitions(snake-game PRIVATE WINDOWS_PLATFORM)
elseif(UNIX)
    target_compile_definitions(snake-game PRIVATE UNIX_PLATFORM)
endif()

if(WIN32)
    target_compile_options(snake-game PRIVATE /W4)
elseif(UNIX)
    target_compile_options(snake-game PRIVATE -Wall -Wextra)
endif()

install(TARGETS snake-game DESTINATION bin)

add_custom_target(run
    COMMAND snake_game
    DEPENDS snake_game
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
