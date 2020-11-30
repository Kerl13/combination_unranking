#ifndef _BENCH_UTILS_H
#define _BENCH_UTILS_H

#include <gmp.h>
#include "../combunrank.h"

void get_random_ranks(mpz_t*, gmp_randstate_t, int n, int k, int nb);
int usage(char progname[]);
unrank_algo_t find_algo(char algo_name[]);

#endif
