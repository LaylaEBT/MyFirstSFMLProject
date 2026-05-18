
# Connecting Dots — A C++ / SFML Learning Project

A 2D puzzle game built with **C++** and **SFML 3**, where the player must
reposition wire endpoints on a grid until every wire lands on its matching
coloured slot.

This project is a personal learning exercise to get comfortable with C++ and
SFML, coming from a C# / Unity background.

---

## 🎯 Concept

The screen shows a 10×10 grid of circular slots connected by thick coloured
wires. The wires start in the wrong positions. The player must drag each wire
endpoint onto the slot whose colour matches the wire, <qwithout stretching the
wire beyond its original length. The game has three levels of increasing
difficulty.

---

## 🕹️ Controls

| Input | Action |
|-------|--------|
| `Z` / `↑` | Move cursor up |
| `S` / `↓` | Move cursor down |
| `Q` / `←` | Move cursor left |
| `D` / `→` | Move cursor right |
| `Enter` | Pick up / drop a wire endpoint |
| Left click near an endpoint | Pick up with mouse |
| Left click on a slot | Drop endpoint |
| Mouse move while dragging | Live wire preview |

---

## 🏗️ Project structure

```
project/
├── src/
│   ├── main.cpp            # Game loop, input handling, draw loop
│   ├── slot.h / .cpp       # Slot class — grid node with colour and state
│   ├── connection.h / .cpp # Connection class — wire between two points
│   ├── DragMode.h          # Enum for drag state
│   ├── GameState.h         # Enum for StartScreen / Playing / Won
│   └── LevelSetup.h / .cpp # All level data — solution slots, barriers, starts
├── assets/
│   ├── font.ttf
│   ├── start.png
│   ├── win.png
│   ├── drop.wav
│   └── win.wav
├── CMakeLists.txt
└── README.md
```

---

## 🔧 Building the project

### Requirements

- [CMake](https://cmake.org/) 3.28 or higher
- A C++17 compatible compiler (MSVC, GCC, or Clang)
- An internet connection for the first build

### Steps

```bash
git clone <your-repo-url>
cd <your-repo-folder>
cmake -B build
cmake --build build
./build/bin/main
```

On **Windows with Visual Studio 2022**, open the folder directly — Visual
Studio detects `CMakeLists.txt` automatically. Select `x64-Release` and build
with `Ctrl+Shift+B`.

SFML 3.0.2 is downloaded automatically via CMake `FetchContent`.

### Asset folder

Place an `assets/` folder next to the executable.

## 📚 C++ concepts explored

- **Header / source file separation** and the scope resolution operator `::`
- **Encapsulation** — private members, setters and getters, the `m_` naming convention
- **References (`&`)** — pass-by-reference, modifying caller state from inside a function
- **`const` correctness** — read-only method guarantees enforced by the compiler
- **Member initializer lists** — initialising class members before the constructor body runs
- **`std::vector`** — dynamic arrays, `push_back`, `emplace_back`, index management
- **Scoped enums (`enum class`)** — type-safe state representation
- **Lambdas** — inline anonymous functions with capture clauses
- **Forward declarations** — resolving top-to-bottom ordering constraints
- **`static` file-scoped helpers** — keeping implementation details local to a translation unit
- **Named constants and the `k_` convention** — eliminating magic numbers
- **Manual 2D vector math** — perpendicular vectors, normalisation, `TriangleStrip` rendering
- **The game loop** — the explicit clear / update / draw cycle
- **State machines** — driving game flow with `GameState` and `DragMode` enums
- **SFML audio** — `SoundBuffer` and `Sound`, loading assets at startup
- **SFML coordinate mapping** — `mapPixelToCoords` for correct multi-screen mouse input

---

## ✅ Features implemented

- [x] Keyboard and mouse input with pick-up and drop
- [x] Fixed-length wire constraint
- [x] Colour-matched solution slots and win detection
- [x] Valid drop target highlight — pulsing ring on reachable slots
- [x] Hover preview while dragging
- [x] Empty slot barriers for routing puzzles
- [x] Sound feedback on drop and on win
- [x] Start screen and win screen with buttons
- [x] Three levels of increasing difficulty with automatic progression
- [x] Full encapsulation refactor across all classes
- [x] accessibility : Wong colour-blind safe palette used

---

## 📄 Dependencies

| Library | Version | How it is included |
|---------|---------|-------------------|
| [SFML](https://www.sfml-dev.org/) | 3.0.2 | CMake `FetchContent` |
