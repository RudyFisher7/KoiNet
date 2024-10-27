# KoiNet
A simple portable C++ networking library.

# Project Status
1. Very early in development.
2. Only static linking is currently supported.

# Compatability
Any system with Unix socket API or WinSock2 API and >= C++11 should work.

## How to Build
For CMake (>=v3.13), add add_subdirectory(<path-to-koinet-subdir>) to your CMakeLists.txt and link to the KoiNet library.

## Tested Compilers
1. GNU gcc (with WSL)
2. MSVC
3. MinGW

# About
This is an exercise to better learn best practices with low-level network programming.

# Goals
1. Be familiar to those who know the C workflow but easier/simpler to use.
2. Minimize the amount of global namespace pollution (e.g., minimize use of preprocessor statements, etc.).
3. API is exactly the same across each supported platform; user code should not have to change between each platform.
4. Provide access to all features available when possible.
5. Use no exceptions.
6. Minimal-overhead abstraction.
7. Use C++ compliant with standard.
8. Keep binary size small.
9. No warnings if possible.
10. Minimize the amount of jank user needs to deal with.

# Inspiration
*Koi are both pleasing to the eye and resilient.*
