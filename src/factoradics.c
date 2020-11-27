#include <gmp.h>
#include <string.h> // memset
#include <malloc.h>
#include <assert.h>

#include "../combunrank.h"

// ---
// Factoradic numeral system
// ---

void factoradic_decomp(int* dest, int len, const mpz_t u) {
  memset(dest, 0, sizeof(int) * len);

  if (mpz_sgn(u) == 0) return;

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
    mpz_divexact_ui(fact, fact, n);
    n--;
  }

  mpz_clear(fact);
  mpz_clear(f_n);
  mpz_clear(x);
}

void factoradic_recomp(mpz_t dest, int* dec, int len) {
  mpz_t fac;
  mpz_init_set_ui(fac, 1);

  mpz_set_ui(dest, 0);

  for (int i = 1; i < len; i++) {
    mpz_mul_ui(fac, fac, i);
    mpz_addmul_ui(dest, fac, dec[i]);
  }

  mpz_clear(fac);
}

// ---
// Permutation unranking
// ---

static void extract(int* dest, const int* F, int n, int k) {
  tree* P = tree_make(0, n - 1);
  for (int i = 0; i < k; i++) {
    dest[i] = tree_pop(P, F[n - 1 - i]);
  }
  tree_clear(P);
}

void unrank_permutation(int* dest, int n, const mpz_t rank) {
  int* F = calloc(n, sizeof(int));
  factoradic_decomp(F, n, rank);
  extract(dest, F, n, n);
  free(F);
}

// ---
// Combination unranking based on factoradics
// ---

// Naive version: convert the combination rank into a well-chosen permutation
// rank and unrank the permutation.

static void rank_conversion(mpz_t dest, int n, int k, const mpz_t rank) {
  mpz_t fac, u, b;

  mpz_init(fac);
  mpz_fac_ui(fac, n);
  mpz_init(b);
  mpz_init_set(u, rank);
  mpz_set_ui(dest, 0);

  int m = 0;
  int i = 0;

  while (i < k) {
    mpz_bin_uiui(b, n - 1 - m, k - 1 - i);
    if (mpz_cmp(u, b) < 0) {
      mpz_divexact_ui(fac, fac, n);
      mpz_addmul_ui(dest, fac, m);
      i++;
      n--;
    } else {
      mpz_sub(u, u, b);
      m++;
    }
  }

  mpz_clear(fac);
  mpz_clear(u);
  mpz_clear(b);
}

void unrank_factoradics_naive(int* dest, int n, int k, const mpz_t rank) {
  mpz_t u;
  mpz_init(u);
  rank_conversion(u, n, k, rank);
  int* F = calloc(n, sizeof(int));
  factoradic_decomp(F, n, u);
  extract(dest, F, n, k);
  mpz_clear(u);
  free(F);
}

// Optimized version
void unrank_factoradics(int* dest, int n, int k, const mpz_t rank) {
  mpz_t binom, r;
  mpz_init_set(r, rank);
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);
  mpz_mul_ui(binom, binom, n);

  int m = 0;
  int i = 0;

  while (i < k - 1) {
    // Invariant:
    // binom = binomial(n - m - i - 1, k - i - 1) * (n - m - i)
    mpz_divexact_ui(binom, binom, n - m - i);
    if (mpz_cmp(binom, r) > 0) {
      dest[i] = m + i;
      mpz_mul_ui(binom, binom, k - i - 1);
      i++;
    } else {
      mpz_sub(r, r, binom);
      mpz_mul_ui(binom, binom, n - m - k);
      m++;
    }
  }
  if (k > 0) dest[k - 1] = n + mpz_get_si(r) - mpz_get_si(binom);

  mpz_clear(r);
  mpz_clear(binom);
}
