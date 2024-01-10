!#/bin/sh

rm TableSolver
c++ -std=c++20 -I.. -Wall -Wextra -o TableSolver utils.cpp line-solver.cpp system-solver.cpp -lstdc++
