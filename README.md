# sudoku_solver
Very simple command line sudoku solver.

## Usage
Just enter known digits line by line. For spaces use spaces. You can omit string ending spaces.
You can also feed data from **stdin**:

    ./sudoku_solver < 1.txt

## Visualization
Current solver state is printed to **stderr**. You can redirect it to some file:

    ./sudoku_solver < 1.txt 2>solve.log
    
then fix line ending with:

    sed -i 's/\r/\r\n/g' solve.log
    
and view all steps of solving process.

## Compilation
    gcc -Wall -g -O2 sudoku_solver.c -o sudoku_solver

## Notes
- Program stops after first finded solution.
- Solve process uses recursion.
