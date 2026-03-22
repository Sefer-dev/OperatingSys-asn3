# Multithreaded Sudoku Validator

A C program that validates a 9×9 Sudoku solution using **27 POSIX threads** — one for each row, column, and 3×3 subgrid. Built for CS 3305 — Operating Systems at Western University.

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Input Format](#input-format)
- [Example Output](#example-output)
- [Technologies](#technologies)

## Overview

A valid Sudoku solution requires every row, column, and 3×3 subgrid to contain the digits 1–9 exactly once. Rather than checking these constraints sequentially, this program spawns **27 threads in parallel** to validate all constraints concurrently:

| Threads | Responsibility            |
|---------|---------------------------|
| 1–9     | Validate 9 subgrids       |
| 10–18   | Validate 9 rows           |
| 19–27   | Validate 9 columns        |

Each thread independently reports whether its assigned region is valid or invalid. The main thread collects all results and determines the overall validity of the puzzle.

## How It Works

1. The program reads a 9×9 grid of integers from a text file into shared memory.
2. It creates 27 threads using `pthread_create()`, passing each thread an index (0–26) that determines which region to check.
3. Each thread uses a `seen` array to detect duplicate digits in its assigned subgrid, row, or column.
4. Results are stored in a shared `result[27]` array — `1` for valid, `0` for invalid.
5. The main thread calls `pthread_join()` on all 27 threads, then iterates through the results to print per-thread outcomes and a final verdict.

## Getting Started

### Prerequisites

- GCC (or any C99-compatible compiler)
- POSIX threads support (`pthread`)

### Compilation

```bash
gcc -o assignment-3 assignment-3.c -lpthread
```

> The `-lpthread` flag links the POSIX threads library.

## Usage

```
./assignment-3 <filename>
```

| Argument   | Description                                      |
|------------|--------------------------------------------------|
| `filename` | Path to a text file containing a 9×9 Sudoku grid |

## Input Format

The input file should contain 9 rows of 9 space-separated integers. For example:

```
6    2    4    5    3    9    1    8    7
5    1    9    7    2    8    6    3    4
8    3    7    6    1    4    2    9    5
1    4    3    8    6    5    7    2    9
9    5    8    2    4    7    3    6    1
7    6    2    3    9    1    4    5    8
3    7    1    9    5    6    8    4    2
4    9    6    1    8    2    5    7    3
2    8    5    4    7    3    9    1    6
```

Two sample files are included in the repository:

- `valid-sudoku.txt` — a correct Sudoku solution
- `invalid-sudoku.txt` — a puzzle with duplicate values in rows 2 and 3

## Example Output

**Valid solution:**

```
$ ./assignment-3 valid-sudoku.txt
Thread # 1 (subgrid 1) is valid
Thread # 2 (subgrid 2) is valid
Thread # 3 (subgrid 3) is valid
Thread # 4 (subgrid 4) is valid
Thread # 5 (subgrid 5) is valid
Thread # 6 (subgrid 6) is valid
Thread # 7 (subgrid 7) is valid
Thread # 8 (subgrid 8) is valid
Thread # 9 (subgrid 9) is valid
Thread # 10 (row 1) is valid
Thread # 11 (row 2) is valid
Thread # 12 (row 3) is valid
Thread # 13 (row 4) is valid
Thread # 14 (row 5) is valid
Thread # 15 (row 6) is valid
Thread # 16 (row 7) is valid
Thread # 17 (row 8) is valid
Thread # 18 (row 9) is valid
Thread # 19 (column 1) is valid
Thread # 20 (column 2) is valid
Thread # 21 (column 3) is valid
Thread # 22 (column 4) is valid
Thread # 23 (column 5) is valid
Thread # 24 (column 6) is valid
Thread # 25 (column 7) is valid
Thread # 26 (column 8) is valid
Thread # 27 (column 9) is valid
valid-sudoku.txt contains a valid solution
```

**Invalid solution:**

```
$ ./assignment-3 invalid-sudoku.txt
Thread # 1 (subgrid 1) is valid
Thread # 2 (subgrid 2) is valid
...
Thread # 11 (row 2) is INVALID
Thread # 12 (row 3) is INVALID
...
Thread # 27 (column 9) is valid
invalid-sudoku.txt contains an INVALID solution
```

> In this example, rows 2 and 3 contain duplicate digits, which the corresponding threads detect and flag.

## Technologies

- **Language:** C (C99)
- **Concurrency:** POSIX Threads (`pthread_create()`, `pthread_join()`, `pthread_exit()`)
- **Shared State:** Global arrays accessed by worker threads
- **Platform:** Linux (tested on Western University's `compute` server)

## Author

Melvin Roger — University of Western Ontario, CS 3305

## License

This project was completed as a course assignment. Please reference responsibly.
