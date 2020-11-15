#include <gmp.h>
#include "../combunrank.h"

// The "textbook" recursive method.
// This function generates combinations in co-lexicographic order and is
// purposely *not* tail-recursive to show the stack explosion
void _unrank_recursive_method_naive(int* dest, int n, int k, mpz_t rank) {
  // Base case 1: the empty subset
  if (k == 0) return;

  // Base case 2: the "full" subset: fill dest with {0, 1, 2, …, n-1}
  if (n == k) {
    for (int i = 0; i < k; i++) dest[i] = n - i - 1;
    return;
  }

  // Decide whether the last element of dest should be (n-1) or < (n-1).
  mpz_t binom;
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);
  if (mpz_cmp(rank, binom) < 0) {
    // {n-1} + unrank(n - 1, k - 1, rank)
    _unrank_recursive_method_naive(dest + 1, n - 1, k - 1, rank);
    dest[0] = n - 1;
  } else {
    // unrank(n - 1, k, rank - binom)
    mpz_sub(rank, rank, binom);
    _unrank_recursive_method_naive(dest, n - 1, k, rank);
  }
  mpz_clear(binom);
}

void unrank_recursive_method_naive(int* dest, int n, int k, const mpz_t rank) {
  // Co-lex unranking
  mpz_t r;
  mpz_init_set(r, rank);
  _unrank_recursive_method_naive(dest, n, k, r);

  // Fix the ordering
  for (int i = 0; i < k; i++) dest[i] = n - 1 - dest[i];

  mpz_clear(r);
}


void unrank_recursive_method(int* dest, int n, int k, const mpz_t rank) {
  mpz_t binom, r;
  mpz_init_set(r, rank);
  mpz_init(binom);
  mpz_bin_uiui(binom, n - 1, k - 1);
  mpz_mul_ui(binom, binom, n);

  int j = 0; // next number candidate to be inserted in dest
  int i = 0; // number of elements in dest

  // If dest is full, we are done.
  // If the rank is 0, we finish outside of the loop
  while ((i < k) & (mpz_sgn(r) > 0)) {
    mpz_divexact_ui(binom, binom, n - j);

    if (mpz_cmp(r, binom) < 0) {
      dest[i] = j;
      mpz_mul_ui(binom, binom, k - i - 1);
      i++;
    } else {
      mpz_sub(r, r, binom);
      mpz_mul_ui(binom, binom, n - j - k + i);
    }
    j++;
  }

  // In case we exited the loop early because r is 0, we fill the end of
  // destination array with the sequence [j, j+1, j+3, …]
  for (; i < k; i++) {
    dest[i] = j;
    j++;
  }

  mpz_clear(r);
  mpz_clear(binom);
}
