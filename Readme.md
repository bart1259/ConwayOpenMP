# Conway's Game of Life accelerated by OpenMP

This is c++ program that shows how OpenMP can accelerate a program by over 50% with minimal additional complexity.

## Compiling with OpenMP

To compile, run the command
    
    g++ -fopenmp -DUSE_OPENMP gameOfLife.cpp -o conway.out

## Compiling wihtout OpenMP
To compile, run the command
    
    g++ gameOfLife.cpp -o conway.out

## Running the program
To run the program, run

    ./conway.out 5000

to run 5000 iterations of Conway's Game of Life

## Results

 | Iterations | Runtime w/o OpenMP (s) | Runtime w/ OpenMP (s) | % faster
|-|-|-|-
 | 1000 | 0.956| 0.618 | 54.6%
 | 10000 | 10.320| 6.652 | 55.1%
 | 100000 | 137.713| 93.963 | 46.5%

**Average improvment: 52%**