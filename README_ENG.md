# C Programming Projects Portfolio

[Russian Version](./README_RUS.md)

Repository for storing projects written in the C language.

## Project Directory

```text
.
├── s21_matrix/         # Matrix operations library (C11, Check.h)
│   ├── tests/          # Unit tests
│   ├── Makefile        # Build and test script
│   ├── *.c, *.h        # Source code and header files
│   ├── README_RUS.md   # Project documentation in Russian
│   └── README_ENG.md   # Project documentation in English
├── README_RUS.md       # Current repository guide in Russian
└── README_ENG.md       # Current repository guide in English
```

## Project List

* **[s21_matrix](./s21_matrix/README_ENG.md)** — Matrix operations library. Implements basic arithmetic operations, transposition, determinant calculation, and inverse matrix computation. The code is compliant with Google Style, thoroughly checked for memory leaks, and covered with unit tests via Check.h.

## General Repository Guidelines

* **Language:** C11 (GCC) for Linux and macOS.
* **Style:** Code formatting via clang-format according to Google Style.
* **Memory:** Mandatory leak checks (Valgrind / leaks).
* **Testing:** Each library is built with unit tests and code coverage reports (gcov / lcov).
