# Comparison of unranking algorithms for combinations

- `make`: build a static library implementing all the algorithms and an
  executable `build/unrank` meant for quick testing

- `make test`: run correctness checks

- `./bench/k_varies.sh ALGO1 ALGO2 …`: compare the given algorithms for a fixed
  n (n = 10000) when k varies (from 0 to n).
  Run `build/unrank` to get a list of all possible algorithms.

- `./bench/n_varies.sh ALGO1 ALGO2 …`: compare the given algorithms when n
  varies from 0 to 10000 and k = n / 2.

## Which algorithm should I use?

Any of the optimised ones. We use our factoradic-based algorithm, which we
release under the permissive MIT licence.

```C
void unrank_factoradics(int* dest, int n, int k, const mpz_t rank) {
  mpz_t binom, r;
  mpz_init_set(r, rank);
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);

  int m = 0;
  int i = 0;

  while (i < k - 1) {
    if (mpz_cmp(binom, r) > 0) {
      dest[i] = m + i;
      mpz_mul_ui(binom, binom, k - i - 1);
      i++;
    } else {
      mpz_sub(r, r, binom);
      mpz_mul_ui(binom, binom, n - m - k);
      m++;
    }
    mpz_divexact_ui(binom, binom, n - m - i);
  }
  if (k > 0) dest[k - 1] = n + mpz_get_si(r) - mpz_get_si(binom) * (n - m - k + 1);

  mpz_clear(r);
  mpz_clear(binom);
```
