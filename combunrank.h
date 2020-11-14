#ifndef _UNRANK_REC_METH
#define _UNRANK_REC_METH

#include <gmp.h> // mpz_*


// Recursive method
void unrank_recursive_method_naive(int* dest, int n, int k, const mpz_t rank);
void unrank_recursive_method(int* dest, int n, int k, const mpz_t rank);

// Factoradic approach
void factoradic_decomp(int* dest, int len, const mpz_t rank);
void factoradic_recomp(mpz_t dest, int* dec, int len);
void unrank_permutation(int* dest, int n, const mpz_t rank);
void unrank_factoradics_fast(int* dest, int n, int k, const mpz_t rank);

#endif
