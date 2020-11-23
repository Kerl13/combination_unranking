#include <gmp.h>
#include "utils.h"

void get_random_ranks(mpz_t* r, gmp_randstate_t s, int n, int k, int nb) {
  mpz_t binom;
  mpz_init(binom);
  mpz_bin_uiui(binom, n, k);

  for (int i = 0; i < nb; i++) {
    mpz_urandomm(r[i], s, binom);
  }

  mpz_clear(binom);
}
