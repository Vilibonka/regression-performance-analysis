## C++ Curve Fitting and Regression with Performance Analysis ##

This project demonstrates the implementation of linear and polynomial regression in C++ using a synthetic quadratic dataset. The focus is both on the learning process (curve fitting) and the performance evaluation of different versions of the program using Linux benchmarking tools.

The dataset is generated from the quadratic function:
  y=0.05x^2+3x+noise
  where noise is randomly sampled to simulate real-world measurement errors.

⚡ The program does not store learned parameters persistently; each execution trains the model again from scratch.

Features:
  - Linear Regression (least squares method)
  - Polynomial Regression (degree 2) for quadratic fitting
  - Multithreading support using std::thread
  - Sample-based regression tests (training on random subsets of the data)
  - Performance analysis using Linux tools:
    1) gprof (profiling)
    2) valgrind (memory usage)
    3) time (execution time)
    4) mpstat / htop (CPU utilization)


Versions:
  - v1 → Basic linear regression implementation
  - v2 → Added polynomial regression (quadratic fitting)
  - v3 → Introduced sampling functions (training on subsets of data)
  - v4 → Parallel regression with multithreading (std::thread) and Performance-optimized version with Linux benchmarking and analysis


Project Goal:
  The main goal of this project is to explore:
  - Curve fitting in C++ using both linear and quadratic models.
  - The impact of noise on regression performance.
  - How different implementations (single-threaded, sampled data, multithreaded) affect accuracy (MSE) and performance (execution time, CPU usage, memory).


Example Outputs:
  - Linear Regression MSE: ~13,910,000 (poor fit for quadratic data)
  - Polynomial Regression MSE: ~99 (close to real quadratic model)
  - Sampled Data Regression MSE: varies depending on subset size (larger samples ≈ better accuracy)


Tools & Environment:
  - Language: C++17
  - Build System: g++ with -O2 and -O3 optimization flags
  - Benchmarking Tools: gprof, valgrind, perf, mpstat, htop
