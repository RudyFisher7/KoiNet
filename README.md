# KoiNet
A simple portable C++ networking library designed for video games.

# Project Status
Very WIP.

# Compatability
Any system with Unix socket API or WinSock2 API, and >= C++11 should work.
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

# Inspiration
*Koi are both pleasing to the eye and resilient.*
