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

void unrank_combinadics_naive2(int* dest, int n, int k, const mpz_t rank) {
  mpz_t r, b;
  mpz_init(r);
  mpz_init(b);

  int d = 0;
  for (int i = 0; i < k - 1; i++) {
    d++;
    mpz_bin_uiui(b, n - d, k - i - 1);
    mpz_add(r, r, b);
    while (mpz_cmp(rank, r) >= 0) {
      d++;
      mpz_bin_uiui(b, n - d, k - i - 1);
      mpz_add(r, r, b);
    }
    mpz_sub(r, r, b);
    dest[i] = d - 1;
  }
  mpz_sub(r, rank, r);
  dest[k - 1] = d + mpz_get_si(r);

  mpz_clear(b);
  mpz_clear(r);
}
