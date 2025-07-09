# snake-game-cpp

A little TUI snake game that I programmed to improve my C++ and CMake skills.

# Installation instructions

## UNIX-like systems
1. Ensure that you have `CMake`, `Git`, and a C++ compiler installed.
2. Clone repository `git clone https://github.com/kotique123/snake-game-cpp.git`.
3. Go to a cloned repository `cd snake-game-cpp`.
4. Create and go to `build` directory `mkdir build && cd build`.
5. Build the game `cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build .`.
6. The binary will be located in the build folder. Run it with `./snake-game`.

## Windows
1. Ensure that you have `CMake`, `Git`, and a C++ compiler installed. I tested the build using the default compiler that comes with
Visual Studio Community edition. I also used the default Windows 11 terminal app with a PowerShell profile.
2. Clone repository `git clone https://github.com/kotique123/snake-game-cpp.git`.
3. Go to a cloned repository `cd snake-game-cpp`.
4. Create and go to `build` directory `mkdir build && cd build`.
5. Generate CMake files and build the game `cmake .. && cmake --build . --config release`.
6. `snake-game.exe` file will be located in `build/Release` directory.