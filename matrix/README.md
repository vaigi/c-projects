# matrix: C Library for Matrix Operations

A proprietary library developed in C (C11 standard) for manipulating numerical matrices. The project encompasses a complete development lifecycle: from safe dynamic memory allocation to the computation of inverse matrices and the implementation of a rigorous cross-platform testing suite.

## Tech Stack and Specification
* **Language:** C (C11 standard).
* **Compiler:** GCC.
* **Code Style:** Google Style Guide (clang-format).
* **Build System:** Makefile (supporting cross-platform compilation on macOS / Linux).
* **Testing:** Unit testing via Check.h framework (with subunit support on Linux).
* **Coverage Analysis:** gcov, lcov (with branch coverage tracking enabled via lcov_branch_coverage=1).

## Memory Structure
A matrix is implemented as a structure holding a two-dimensional dynamic array (an array of pointers to double-typed rows):

```c
typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;
```

---

## Function Reference and Return Codes

All core functions (except for matrix comparison) return an integer execution status code:
* **0 (OK)** — Operation completed successfully.
* **1 (INCORRECT_MATRIX)** — Invalid matrix structure (pointer is NULL, dynamic memory is unallocated, or dimensions are less than or equal to 0).
* **2 (CALC_ERROR)** — Mathematical calculation error (mismatched matrix dimensions during arithmetic operations, dimension mismatch between columns and rows during matrix multiplication, or determinant calculation attempted on a non-square matrix).

### 1. Memory Management
* `int s21_create_matrix(int rows, int columns, matrix_t *result);` — Allocates memory for rows and columns.
* `void s21_remove_matrix(matrix_t *A);` — Safely performs cascaded memory deallocation (free) and nullifies all structure fields to prevent memory leaks.

### 2. Matrix Comparison
* `int s21_eq_matrix(matrix_t *A, matrix_t *B);` — Performs element-wise comparison.
  * Returns: SUCCESS (1) — matrices are equal, FAILURE (0) — matrices are not equal.
  * Floating-point precision (double) comparison is implemented using fabs up to 6 decimal places inclusive (1e-6).

### 3. Arithmetic and Linear Transformations
* `int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);` — Performs addition of matrices with identical dimensions.
* `int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);` — Performs subtraction of matrices with identical dimensions.
* `int s21_mult_number(matrix_t *A, double number, matrix_t *result);` — Multiplies each matrix element by a scalar value.
* `int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);` — Performs matrix multiplication. Validates the condition that the number of columns in the first matrix must equal the number of rows in the second matrix.

### 4. Linear Algebra Algorithms
* `int s21_transpose(matrix_t *A, matrix_t *result);` — Transposes the matrix (swaps rows with columns).
* `int s21_determinant(matrix_t *A, double *result);` — Recursively calculates the determinant of a square matrix using row expansion.
* `int s21_calc_complements(matrix_t *A, matrix_t *result);` — Computes the algebraic cofactor matrix using minor signs and recursive minor determinant calculation.
* `int s21_inverse_matrix(matrix_t *A, matrix_t *result);` — Computes the inverse matrix. Returns CALC_ERROR (2) if the determinant is 0.

### Internal Decomposition (Helper Functions)
To maintain architectural clarity and code reusability, several private helper functions are implemented within the library:
* `s21_check_matrix` — Comprehensive validation of the matrix structure.
* `s21_check_matrix_size` — Validation of dimension matching between two matrices.
* `s21_get_minor_matrix` — Generates a truncated matrix (minor) by cross-striking a specified row and column.
* `s21_calculate_determinant` — Core recursive engine for calculating determinants of N by N matrices.

---

## Automation and Build Instructions

Compilation, testing, and linting processes are fully automated via Makefile.

* `make all` / `make` — Compiles the project (default target, builds the static library).
* `make s21_matrix.a` — Compiles the project source files and archives them into a static library using ar rcs.
* `make test` — Compiles and automatically executes unit tests located in the tests directory using the Check utility.
* `make gcov_report` — Conducts code coverage analysis. Rebuilds the project with --coverage flags, executes tests, aggregates lcov statistics filtering out the test modules themselves, includes branch coverage metrics, and generates an interactive HTML report inside the report folder. Automatically opens the report in a web browser using xdg-open or open.
* `make leaks` — Executes the test binary under the macOS leaks utility to verify that dynamic memory is free of leaks upon program termination.
* `make clang` — Copies the configuration file and automatically formats all source code files across all directories strictly according to the Google Style standard.
* `make clean` — Completely flushes the build directory by removing object files, static libraries, test binaries, temporary coverage files, and generated HTML reports.
