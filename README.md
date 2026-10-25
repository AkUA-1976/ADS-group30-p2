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

The five road networks come from the 9th DIMACS Implementation Challenge
(Shortest Paths). They can be downloaded from the official page:

http://www.dis.uniroma1.it/challenge9/download.shtml

The files used are `USA-road-d.NY.gr`, `USA-road-d.BAY.gr`,
`USA-road-d.COL.gr`, `USA-road-d.FLA.gr` and `USA-road-d.CAL.gr`. The data
sets are not part of the submission; only the download link is listed.

Put the `.gr` files in `data/`. The reader also searches `../data/` and
`../../data/`, so the programs can be run from the repository root, from
`build/`, or from `build/Release/`.

## Source layout

- `include/`: graph reader, the two heap structures, and the two Dijkstra
  implementations.
- `src/`: the implementation files and the four programs.
- `test/`: the small hand-checkable graph `test.gr`.
- `report/`: the lab report and its figure (included in the submission zip).

## Programs

- `app`: reads NY and checks the number of arcs.
- `test`: correctness test on the small graph `test/test.gr`.
- `benchmark`: NY symmetry test with both heap implementations.
- `perf`: performance test with 1000 random query pairs per data set, using the
  same fixed-seed query list for both heaps. It reports total time, average
  time, the number of reachable pairs, and a checksum cross-check.
