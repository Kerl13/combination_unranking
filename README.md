# Comparison of unranking algorithms for combinations

- `make`: build a static library implementing all the algorithms and an
  executable `build/unrank` meant for quick testing

- `make test`: run correctness checks

- `./bench/k_varies.sh ALGO1 ALGO2 …`: compare the given algorithms for a fixed
  n (n = 10000) when k varies (from 0 to n).
  Run `build/unrank` to get a list of all possible algorithms.

- `./bench/n_varies.sh ALGO1 ALGO2 …`: compare the given algorithms when n
  varies from 0 to 10000 and k = n / 2.
