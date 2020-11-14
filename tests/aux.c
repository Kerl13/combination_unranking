#include <gmp.h>
#include <malloc.h>
#include <assert.h>
#include "../combunrank.h"


void manual_tests() {
  mpz_t rank;
  mpz_init_set_ui(rank, 2021);
  int expected[] = {0, 1, 2, 0, 4, 4, 2};

  int* dec = calloc(7, sizeof(int));
  factoradic_decomp(dec, 7, rank);
  for (int i = 0; i < 7; i++) assert(dec[i] == expected[i]);
  free(dec);

  mpz_t x;
  mpz_init(x);
  factoradic_recomp(x, expected, 7);
  assert(mpz_cmp(x, rank) == 0);
  mpz_clear(x);

  mpz_clear(rank);
}

void check_decomp_recomp(int n) {
  mpz_t rank, tmp, fac;
  mpz_inits(rank, tmp, fac, NULL);
  mpz_fac_ui(fac, n);
  int* dec = calloc(n, sizeof(int));

  do {
    factoradic_decomp(dec, n, rank);
    factoradic_recomp(tmp, dec, n);
    assert(mpz_cmp(tmp, rank) == 0);
    mpz_add_ui(rank, rank, 1);
  } while (mpz_cmp(rank, fac) < 0);

  mpz_clears(rank, tmp, fac, NULL);
  free(dec);
}

int main() {
  const int N = 7;

  manual_tests();
  for (int n = 2; n <= N; n++) check_decomp_recomp(n);

  return 0;
}
