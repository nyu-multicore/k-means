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
- kmeans_ver3: parallel implementation of [_Accelerating K-Means Clustering with Parallel Implementations and GPU
  computing_](https://ieeexplore.ieee.org/document/7322467)
- kmeans_par1_1: parallel versions including the parallelization of the point assignments
- kmeans_par1_2: parallel versions including the parallelization of the point assignments and center recompute

## Executable Usage

Use `./cmake-build-release/<kmeans_version> -h` to see the usage:

```text
usage: ./cmake-build-release/<kmeans_version> t k filename labels_file

positional arguments:
  t            the number of threads, 0 < t <= 100
  k            the number of clusters, 1 < k <= 100
  filename     the name of the input file that contains data vectors
  labels_file  the name of the file that contains data labels
```

Argument `t` is only valid for parallel versions, and is ignored by the sequential version.

Example:

```bash
./cmake-build-release/kmeans_seq 1 10 data/dataset-1000000.txt data/dataset-1000000.10.kmeans.txt
```

```bash
./cmake-build-release/kmeans 10 10 data/dataset-1000000.txt data/dataset-1000000.10.kmeans.txt
```

## Run Experiments on Datasets

A python script is available to run and time kmeans versions on datasets.

After successfully building the executables, you can run the script with the following command:

```bash
python3 scripts/exp.py
```

This will automatically search for all the executables in the folder `cmake-build-release` and run them on all the
datasets. It will run each version on each dataset with each thread count and each n_cluster, each for 10 times, the raw
data will be save to path `data/experiment_raw.csv`. The averaged data will be save to path `data/experiment_avg.csv`.

### To run only a specific version

```bash
python3 scripts/exp.py -e kmeans_ver3
```
