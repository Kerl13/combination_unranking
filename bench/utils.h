#ifndef _BENCH_UTILS_H
#define _BENCH_UTILS_H

#include <gmp.h>

typedef void (*algo)(int*, int, int, const mpz_t);

void get_random_ranks(mpz_t*, gmp_randstate_t, int n, int k, int nb);

#endif
