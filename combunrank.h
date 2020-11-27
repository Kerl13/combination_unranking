#ifndef _UNRANK_REC_METH
#define _UNRANK_REC_METH

#include <gmp.h> // mpz_*
#include "src/internals.h"

// Recursive method
void unrank_recursive_method_naive(int* dest, int n, int k, const mpz_t rank);
void unrank_recursive_method_tr(int* dest, int n, int k, const mpz_t rank);
void unrank_recursive_method(int* dest, int n, int k, const mpz_t rank);

// Factoradic approach
void factoradic_decomp(int* dest, int len, const mpz_t rank);
void factoradic_recomp(mpz_t dest, int* dec, int len);
void unrank_permutation(int* dest, int n, const mpz_t rank);
void unrank_factoradics_naive(int* dest, int n, int k, const mpz_t rank);
void unrank_factoradics(int* dest, int n, int k, const mpz_t rank);

// Combinadic approach
void unrank_combinadics_naive(int* dest, int n, int k, const mpz_t rank);
void unrank_combinadics_naive2(int* dest, int n, int k, const mpz_t rank);

// The list of all algorithms
typedef void (*unrank_algo_t)(int* dest, int n, int k, const mpz_t rank);
typedef struct name_algo_pair_s {
  char* name;
  unrank_algo_t func;
} name_algo_pair;
extern const name_algo_pair unrank_algo_list[10];

#endif
