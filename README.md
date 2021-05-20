# Parallel-Conway-s-Game-of-Life
A parallel implementation of the Conway's Game of Life with periodic boundaries using Message Passing Interface (MPI).

To compile the parallelized version, run the following (from the golMPI folder):
`mpicc main.c ../util/*.c`

To run: `mpirun -np x ./a.out`, where x is the number of proccesses you need.
