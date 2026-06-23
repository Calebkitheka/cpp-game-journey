# 🎮 CPP Game Journey | Pong Prototype
![CI](https://github.com/yourusername/cpp-game-journey/actions/workflows/ci.yml/badge.svg)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue) ![SFML](https://img.shields.io/badge/SFML-2.6-green) ![License](https://img.shields.io/badge/License-MIT-yellow)

A polished, architecturally sound 2D Pong prototype built in C++ to demonstrate engine fundamentals: state machines, delta-time physics, AABB collision, GPU post-processing, and automated CI/CD.

## 📸 Gameplay
![pong-screenshot](assets/screenshot.png) *(Replace with actual GIF/screenshot)*

## 🛠️ Features
- ✅ Frame-rate independent movement & physics
- ✅ Polymorphic state machine (`Menu` → `Play` → `GameOver`)
- ✅ GPU post-processing (CRT scanlines, vignette, color grading)
- ✅ Persistent JSON config & high-score saving
- ✅ Cross-platform CI/CD pipeline (Win/Mac/Linux)
- ✅ Zero memory leaks (RAII, `unique_ptr`, safe iteration)

## 📦 Build & Run
```bash
git clone https://github.com/yourusername/cpp-game-journey.git
cd cpp-game-journey
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/pong  # Windows: .\build\pong.exe