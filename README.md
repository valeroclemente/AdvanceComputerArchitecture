# AdvanceComputerArchitecture
Parallel Computer: Performance and Power Consumption Evaluation of OpenMP and MPI

The following files implements three different benchmarks to measure performance on MPI and OpenMP. 
Each file was developed on C++.

The Sample Data for all implementation can be found in Sample Data folder
  1. lady1000.bmp (sample data for Image Averaging Filter Benchmark)
  2. float.zip (sample data for Dense Matrix-Matrix Multiplication Benchmark)
  3. data.zip (sample data for Parallel Merge Sort Benchmark)

Folder MPI_windows_codes
  In this folder, you can find the three adapted benchmarks used on MPI. 
  1. FitMPI.c: Image Averaging Filter Benchmark
  2. MulMatMPI.c: Dense Matrix-Matrix Multiplication Benchmark
  3. Merge.c: Parallel Merge Sort Benchmark
  
Folder OpenMP_windows_codes
In this folder, you can find the three adapted benchmarks used on OpenMP. 
  1. FiltOpenmp.c: Image Averaging Filter Benchmark
  2. MulMatOpenMP.c: Dense Matrix-Matrix Multiplication Benchmark
  3. mergeOpenMP.c: Parallel Merge Sort Benchmark

Folder MPI_linux_codes
  In this folder, you can find the three adapted benchmarks used on MPI suitable for Linux. 
  1. FitMPI_linux.c: Image Averaging Filter Benchmark
  2. MulMatMPI_linux.c: Dense Matrix-Matrix Multiplication Benchmark
  3. MergeMPI_linux.c: Parallel Merge Sort Benchmark
  
Folder OpenMP_linux_codes
In this folder, you can find the three adapted benchmarks used on OpenMP suitable for Linux. 
  1. FiltOpenmp_linux.c: Image Averaging Filter Benchmark
  2. MulMatOpenmp_linux.c: Dense Matrix-Matrix Multiplication Benchmark
  3. mergeOpenMP_linux.c: Parallel Merge Sort Benchmark

Folder Native_Threads_C_windows
In this folder, you can find the three adapted benchmarks used on Native C Threads for Windows
  1. ThFil_windows.c: Image Averaging Filter Benchmark
  2. ThMulMat_windows.c: Dense Matrix-Matrix Multiplication Benchmark
  3. ThMerge_windows.c: Parallel Merge Sort Benchmark

