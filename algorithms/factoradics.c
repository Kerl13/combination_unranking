#include <gmp.h>
#include <string.h> // bzero

#include "../combunrank.h"


void factoradic(int* dest, int len, const mpz_t u) {
  bzero(dest, sizeof(int) * len);

  // Invariant: fact = n!
  int n = len - 1;
  mpz_t fact;
  mpz_init(fact);
  mpz_fac_ui(fact, n);

  // Find the unique n such that: n! ≤ u < (n + 1)!
  while (mpz_cmp(fact, u) > 0) {
    mpz_divexact_ui(fact, fact, n);
    n--;
  }

  mpz_t f_n;
  mpz_init(f_n);
  // Invariant: u = x + \sum_{i=n+1}^{len-1} dest[i] * i!
  mpz_t x;
  mpz_init_set(x, u);
  while (mpz_sgn(x) > 0) {
    mpz_tdiv_qr(f_n, x, x, fact);
    dest[n] = mpz_get_si(f_n);
    n--;
  }
}


void unrank_factoradics_fast(int* dest, int n, int k, const mpz_t rank) {
  mpz_t binom, r;
  mpz_init_set(r, rank);
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);
  mpz_mul_ui(binom, binom, n);

  int m = 0;
  int i = 0;

  while (i < k) {
    mpz_divexact_ui(binom, binom, n - m);
    if (mpz_cmp(binom, r) > 0) {
      dest[i] = m + i;
      mpz_mul_ui(binom, binom, k - i - 1);
      i++;
      n--;
    } else {
      mpz_sub(r, r, binom);
      mpz_mul_ui(binom, binom, n - m - k + i);
      m++;
    }
  }
}
