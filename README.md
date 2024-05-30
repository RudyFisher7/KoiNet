# KoiNet
A simple portable C++ networking library designed for video games.

# Project Status
Very WIP.

# About
This is an exercise to better learn best practices with low-level network programming.

# Goals
1. Minimize the amount of global namespace pollution (e.g., minimize use of preprocessor statements, etc.).
2. API is exactly the same across each supported platform; user code should not have to change between each platform.
3. Provide access to all features available when possible.
4. Use no exceptions.
5. Zero-overhead abstraction.
6. Use C++ compliant with standard.
7. Keep binary size small.
