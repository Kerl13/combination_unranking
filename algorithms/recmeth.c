#include <gmp.h>
#include "../combunrank.h"


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
}
