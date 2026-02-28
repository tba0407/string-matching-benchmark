# 2D Pattern Matching Benchmark

This repository contains a benchmark suite designed to evaluate and compare the performance behavior of various string matching algorithms on a 2D character grid under extreme load conditions.

## Algorithms & Implementation Details
* **Naive (Brute Force):** Standard character-by-character 2D search.
* **Rabin-Karp:** Implemented using a 2D Rolling Hash.
* **Boyer-Moore:** Utilizes the Bad Character Heuristic.
* **Aho-Corasick:** Implemented as a Deterministic Finite Automaton (DFA) for true multi-pattern matching.

## Test Cases Distribution
The benchmark consists of 120 aggressively generated test cases. Each test uses a $1000 \times 1000$ character grid and searches for up to $10,000$ keywords of varying lengths. The dataset is divided into three distinct scenarios to observe algorithm behaviors in both average and worst-case environments:
* **Tests 1-20 (Anti-Hash):** Designed to force massive hash collisions, acting as a worst-case scenario for Rabin-Karp.
* **Tests 21-40 (Anti-Jump):** Both grid and keywords are filled with uniform, repeating characters (e.g., all `'a'`) to neutralize Boyer-Moore's skipping advantage.
* **Tests 41-120 (Natural Distribution):** Randomized grids and pattern lengths to simulate average, real-world text search scenarios.

## Benchmark Environment
**Hardware Specifications:**
* **Model:** Asus ROG Zephyrus G14 (2025)
* **CPU:** AMD Ryzen™ AI 9 HX 370
* **RAM:** 32GB LPDDR5X-8000MHz
* **OS:** Windows 11 64-bit
* **Compiler:** GCC (MinGW-w64) C++23

**System Condition During Benchmark:**
* Device plugged in and set to High-Performance mode.
* No heavy background processes or IDEs running during the execution.
* Test cases were pre-generated and read from the local SSD to ensure I/O operations did not bottleneck the actual CPU timing phase.

## Results Visualization
### Execution Time
<img width="1600" height="800" alt="Code_Generated_Image" src="https://github.com/user-attachments/assets/60c4f166-4cbe-4a9f-a02a-a1b421d62690" />


### Character Comparisons
<img width="1600" height="800" alt="Code_Generated_Image (1)" src="https://github.com/user-attachments/assets/690aa80e-168a-42c4-b9f9-1d45a482b8af" />
