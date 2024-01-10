!#/bin/sh

rm LineSolver
c++ -std=c++20 -I.. -Wall -Wextra -o LineSolver utils.cpp line-solver.cpp line-solver-test.cpp -lstdc++
