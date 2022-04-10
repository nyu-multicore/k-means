# k-means

Parallel K-Means(H-Means) Implementation in OpenMP.

## Prerequisites

- CMake 3.17+
- GCC 9.2+
- OpenMPI 4+

## Getting Started

### Switch Modules on CIMS machines

```bash
module load cmake-3
module load gcc-9.2
```

### Build

```bash
cmake -DCMAKE_BUILD_TYPE=Release
make
```

This would build different versions of executables:

- kmeans_seq

## Executable Usage

Use `./<kmeans_version> -h` to see the usage:

```
usage: ./<kmeans_version> t k filename

positional arguments:
  t          the number of threads, 0 < t <= 100
  k          the number of clusters, 1 < k <= 100
  filename   the name of the input file that contains data vectors
```

Argument `k` is only valid for parallel versions, and is ignored by the sequential version.

Example:

```bash
./kmeans_seq 1 10 data/dataset-1000000.txt
```

```bash
./kmeans 10 10 data/dataset-1000000.txt
```
