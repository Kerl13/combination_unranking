- `make`: build a static library implementing all the algorithms and an
  executable `build/unrank` meant for quick testing
  
- `make test`: run correctness checks

- `make bench`: compare the best algorithms

- `./bench/k_varies.sh ALGO1 ALGO2 ...`: compare any given set of algorithms.
  Run `build/unrank` to get a list of all possible algorithms.
