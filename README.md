# ADS-group30-p2

Dijkstra's algorithm with a binomial queue and a Fibonacci heap.

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

The executables are `app`, `test`, `benchmark`, and `perf`. With Visual Studio
they are placed in `Release/`; with MinGW they are placed in `build/`.

## Data

Download the five DIMACS road networks from the release:

https://github.com/AkUA-1976/ADS-group30-p2/releases/tag/dataset

Put the `.gr` files in `data/`. The reader also searches `../data/` and
`../../data/`, so the programs can be run from the repository root, from
`build/`, or from `build/Release/`.

## Programs

- `app`: reads NY and checks the number of arcs.
- `test`: correctness test on the small graph `test/test.gr`.
- `benchmark`: NY symmetry test with both heap implementations.
- `perf`: performance test with 1000 random query pairs per data set, using the
  same fixed-seed query list for both heaps. It reports total time, average
  time, the number of reachable pairs, and a checksum cross-check.
