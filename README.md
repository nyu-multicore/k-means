# k-means

Parallel K-Means(H-Means) implementations in OpenMP.

## Prerequisites

- CMake 3.17+
- GCC 9.2+
- OpenMPI 3+

## Recommended IDE

CLion 2021.3: [Download](https://www.jetbrains.com/clion/)

## Getting Started

### Switch Modules on CIMS machines

```bash
module load cmake-3
module load gcc-9.2
```

### Build

```bash
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --config Release
```

This would build and generate different versions of executables in the folder `cmake-build-release`.

- kmeans_seq: basic sequential version

## Executable Usage

Use `./cmake-build-release/<kmeans_version> -h` to see the usage:

```text
usage: ./cmake-build-release/<kmeans_version> t k filename

positional arguments:
  t          the number of threads, 0 < t <= 100
  k          the number of clusters, 1 < k <= 100
  filename   the name of the input file that contains data vectors
```

Argument `k` is only valid for parallel versions, and is ignored by the sequential version.

Example:

```bash
./cmake-build-release/kmeans_seq 1 10 data/dataset-1000000.txt
```

```bash
./cmake-build-release/kmeans 10 10 data/dataset-1000000.txt
```
