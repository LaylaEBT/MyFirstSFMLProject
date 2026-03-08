
This repository is my first SMFL project using the SFML project Template.
The purpose of this project is to make a game out of it, but mainly to learn!
# Wire Untangle — SFML Learning Project

A 2D puzzle game built with **C++** and **SFML 3**, where the player must untangle a set of crossed wires by repositioning their endpoints on a grid.

This project is a personal learning exercise to get comfortable with **C++** and **SFML** coming from a **C# / Unity** background.

---

## 🎮 Concept

The game is inspired by classic *wire connect* / *untangle* puzzle games. The screen displays a grid of slots connected by colored wires (the `Connection` objects). The wires are initially shuffled and cross each other. The player's goal is to move the wire endpoints around the grid until no wires are crossing.

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| `Z` / `↑` | Move cursor up |
| `S` / `↓` | Move cursor down |
| `Q` / `←` | Move cursor left |
| `D` / `→` | Move cursor right |
| `Enter` | Select a wire endpoint / confirm new position |

**Two-step interaction:**
- Press `Enter` on a slot that holds a wire endpoint to **select** it (the wire turns red).
- Navigate to a new slot and press `Enter` again to **move** the endpoint there.

---

## 🏗️ Project Structure

```
project/
├── src/
│   ├── main.cpp          # Entry point, game loop, grid setup
│   ├── slot.h            # Slot class declaration
│   ├── slot.cpp          # Slot class implementation
│   ├── connection.h      # Connection class declaration
│   └── connection.cpp    # Connection class implementation
├── CMakeLists.txt
└── README.md
```

### Key Classes

**`Slot`** — represents a single node on the grid. It knows its position and whether it is currently selected by the cursor. It draws itself as a green circle (blue when selected).

**`Connection`** — represents a wire between two slots. It stores a start and end point and draws itself as a thick yellow line (red when selected/being moved).

---

## 🔧 Building the Project

### Requirements

- [CMake](https://cmake.org/) 3.28 or higher
- A C++17 compatible compiler (MSVC, GCC, or Clang)
- An internet connection for the first build (SFML is fetched automatically)

### Steps

```bash
# Clone the repository
git clone <your-repo-url>
cd <your-repo-folder>

# Configure
cmake -B build

# Build
cmake --build build

# Run
./build/bin/main
```

SFML 3.0.2 is automatically downloaded and compiled via CMake's `FetchContent` — no manual installation needed.

---

## 📚 What I Am Learning

This project is my hands-on introduction to C++ after working in C# with Unity. Key concepts I am exploring:

- **Header / source file separation** — declaring classes in `.h` files and implementing them in `.cpp` files
- **References (`&`)** — passing objects by reference instead of by value, equivalent to how C# handles class instances automatically
- **The scope operator (`::`)** — linking method implementations back to their class outside of the class body
- **`const` correctness** — marking methods that don't modify the object
- **`std::vector`** — C++'s equivalent of C#'s `List<T>`
- **The game loop** — the clear / update / draw cycle that drives real-time applications in SFML
- **Manual 2D math** — drawing thick lines using perpendicular vectors and triangle strips, something Unity's built-in renderer would normally handle automatically
- And many more to follow!

---

## 🗺️ Planned Features

- [ ] Win condition — detect when no wires are crossing
- [ ] Randomised wire layouts on startup
- [ ] Visual feedback when two wires intersect
- [ ] Multiple levels with increasing complexity

---

## 📄 Dependencies

| Library | Version | How it is included |
|---------|---------|-------------------|
| [SFML](https://www.sfml-dev.org/) | 3.0.2 | Auto-downloaded via CMake FetchContent |
