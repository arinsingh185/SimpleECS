# SimpleECS Movement System
### A little side project in learning about Cache Locality and SIMD vectorization

## Project Overview
This project is a high-performance implementation of a 2D Movement System, designed to explore the limits of data-oriented design and hardware-aware C++. The goal was to optimize a standard "scalar" movement loop using **Structure of Arrays (SoA)** and **AVX2 SIMD intrinsics**.

## Technical Implementation

### 1. Memory Layout (SoA vs. AoS)
Instead of a traditional Array of Structures (AoS), I implemented a **Structure of Arrays (SoA)**. This ensures that all `posX` values are contiguous in memory, maximizing the CPU's cache-line utilization and allowing for efficient prefetching.

### 2. Cache-Line Alignment
To support high-throughput SIMD loads, I utilized `_aligned_malloc` to ensure all component arrays start on a **64-byte boundary**. This aligns our data with the CPU’s L1 cache lines, preventing "split-load" penalties.

### 3. SIMD Vectorization (AVX2)
The core `MovementSystem` was refactored using Intel/AMD intrinsics (`immintrin.h`). By using **256-bit YMM registers**, the system processes **8 entities simultaneously** in a single clock cycle.

## Benchmarks & Results
*Tested on: x64 Windows Environment*
*Configuration: 1,000 Frames of Simulation*

| Entity Count | Scalar (Baseline) | AVX2 SIMD | Speedup |
| :--- | :--- | :--- | :--- |
| 100,000 | 301.3 ms | 40.0 ms | **~7.5x** |
| 1,000,000 | ~3,000 ms* | 550.0 ms | **~5.5x** |

*\*Estimated based on 100k baseline.*

---

## How to Build
1. Open the project in Visual Studio.
2. Set configuration to **Release / x64**.
3. Enable **AVX2** in Project Properties:
   * **Project Properties** -> **C/C++** -> **Code Generation** -> **Enable Enhanced Instruction Set** -> **Advanced Vector Extensions 2 (/arch:AVX2)**.
4. Build and Run.
