#include <gmp.h>

#include "../combunrank.h"


void unrank_combinadics_naive(int* dest, int n, int k, const mpz_t rank) {
  // u <- binomial(n, k) - 1 - rank
  mpz_t u;
  mpz_init(u);
  mpz_bin_uiui(u, n, k);
  mpz_sub_ui(u, u, 1);
  mpz_sub(u, u, rank);

  mpz_t b;
  mpz_init(b);

  int v = n;

  for (int i = 0; i < k; i++) {
    v--;
    mpz_bin_uiui(b, v, k - i);
    while (mpz_cmp(u, b) < 0) {
      v--;
      mpz_bin_uiui(b, v, k - i);
    }
    mpz_sub(u, u, b);
    dest[i] = n - 1 - v;
  }

  mpz_clear(u);
  mpz_clear(b);
}
