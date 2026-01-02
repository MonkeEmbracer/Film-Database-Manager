# 🎬 Film Database Manager (C++ / Qt)

Desktop film database application built in C++ with Qt, featuring CRUD operations, undo/redo functionality, and a genre-based recommendation system.

## 🧭 Overview
This project is a GUI-based application for managing a collection of films. It follows a layered structure (domain -> repository -> service -> UI/GUI) and supports multiple storage formats.

## ✨ Features
- CRUD operations for films
- Qt graphical interface
- Undo / redo support for operations
- Genre-based film discovery system
- Multiple storage formats (file-based)

## 🛠️ Requirements
- C++17 compatible compiler
- CMake
- Qt 6
- WSL or a UNIX-based system
- CLion recommended

### 🐧 If you use WSL (recommended on Windows)
- A working WSL toolchain configured in CLion
- Qt installed inside the WSL environment

## ▶️ Build & Run (CLion + WSL)
1. Open the project folder in **CLion**
2. Select the **WSL toolchain**
3. Let CMake configure automatically
4. Build and run the target configured by CLion

> The entry point is `Start.cpp`.

## 🧩 Diagram
A project diagram is included as `diagram.png` (useful for architecture overview).
