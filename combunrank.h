#ifndef _UNRANK_REC_METH
#define _UNRANK_REC_METH

#include <gmp.h> // mpz_*


// Recursive method
void unrank_recursive_method_naive(int* dest, int n, int k, const mpz_t rank);
void unrank_recursive_method(int* dest, int n, int k, const mpz_t rank);

// Factoradic approach
void unrank_factoradics_fast(int* dest, int n, int k, const mpz_t rank);

#endif
