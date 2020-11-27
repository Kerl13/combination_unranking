// Wee command line tool to unrank combinations

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include "combunrank.h"


static const unsigned int nb_algos =
  sizeof(unrank_algo_list) / sizeof(name_algo_pair);

static unrank_algo_t get_algorithm(char name[]) {
  for (unsigned int i = 0; i < nb_algos; i++) {
    if (strcmp(unrank_algo_list[i].name, name) == 0)
      return unrank_algo_list[i].func;
  }
  return NULL;
}


static void print_combination(const int* comb, const int k) {
  for (int i = 0; i < k - 1; i++) {
    printf("%d, ", comb[i]);
  }
  if (k > 0) printf("%d", comb[k - 1]);
  printf("\n");
}


// ---
// CLI parsing
// ---

typedef struct {
  int n;
  int k;
  mpz_t rank;
  char* algo;
} cli_options;

static int usage(char progname[]) {
  fprintf(stderr, "usage: %s -n N -k K [-r RANK] [-a ALGORITHM]\n", progname);
  fprintf(stderr, "unrank the (N, K)-combination number RANK in lexicographic order\n");
  fprintf(stderr, "  -r RANK       the rank of the combination, if set to -1 unrank all\n"
                  "                combination in lexicographic order (defaults to -1)\n");
  fprintf(stderr, "  -a ALGORITHM  the unranking algorithm to be used, possible values are:\n"
                  "                ");
  for (unsigned int i = 0; i < nb_algos - 1; i++) {
    fprintf(stderr, "%s,", unrank_algo_list[i].name);
    if (i % 3 == 2) fprintf(stderr, "\n                ");
    else fprintf(stderr, " ");
  }
  fprintf(stderr, "%s\n", unrank_algo_list[nb_algos - 1].name);

  return 1;
}

static int parse_pos_int(char varname, char* s) {
  char* endptr;
  long res = strtol(s, &endptr, 10);
  if (endptr == s)
    return -1;
  else if (res > INT_MAX) {
    fprintf(stderr, "error: %c is too big (> %d).\n", varname, INT_MAX);
    return -1;
  } else if (res <= 0) {
    fprintf(stderr, "error: %c must be positive (got %d).\n", varname, (int)res);
    return -1;
  } else
    return (int)res;
}


static cli_options cli_parse(int argc, char* argv[]) {
  cli_options opts;
  mpz_init_set_si(opts.rank, -1);
  opts.n = -1;
  opts.k = -1;
  opts.algo = "recursive_method";

  int c;
  while ((c = getopt(argc, argv, "n:k:r:a:")) != -1) {
    switch (c) {
      case 'n':
        opts.n = parse_pos_int('n', optarg);
        break;
      case 'k':
        opts.k = parse_pos_int('k', optarg);
        break;
      case 'r':
        if (mpz_set_str(opts.rank, optarg, 10) == -1) {
          fprintf(stderr, "error: invalid integer %s\n", optarg);
          exit(usage(argv[0]));
        }
        break;
      case 'a':
        opts.algo = optarg;
        break;
      default:
        abort();
    }
  }

  if (opts.n == -1) {
    fprintf(stderr, "error: -n is missing\n");
    exit(usage(argv[0]));
  }
  if (opts.k == -1) {
    fprintf(stderr, "error: -k is missing\n");
    exit(usage(argv[0]));
  }

  return opts;
}

int main(int argc, char* argv[]) {
  cli_options opts = cli_parse(argc, argv);

  // Sanity checks
  if (opts.k > opts.n) {
    fprintf(stderr, "error: k too big, should be at most n\n");
    return usage(argv[0]);
  }
  mpz_t binom;
  mpz_init(binom);
  mpz_bin_uiui(binom, opts.n, opts.k);

  if (mpz_cmp_si(opts.rank, -1) < 0 || mpz_cmp(opts.rank, binom) >= 0) {
    fprintf(stderr, "error: invalid rank, should be in [0; binomial(n, k)]\n");
    fprintf(stderr, "       got: ");
    mpz_out_str(stderr, 10, opts.rank);
    fprintf(stderr, "\n       binomial(%d, %d) is ", opts.n, opts.k);
    mpz_out_str(stderr, 10, binom);
    fprintf(stderr, "\n");
    return usage(argv[0]);
  }

  // Select the algorithm
  unrank_algo_t algo = get_algorithm(opts.algo);
  if (algo == NULL) {
    fprintf(stderr, "error: unknown algorithm %s\n", opts.algo);
    return usage(argv[0]);
  }

  int* comb = calloc(opts.k, sizeof(int));

  if (mpz_cmp_si(opts.rank, -1) == 0) {
    mpz_set_si(opts.rank, 0);
    while (mpz_cmp(opts.rank, binom) < 0) {
      algo(comb, opts.n, opts.k, opts.rank);
      print_combination(comb, opts.k);
      mpz_add_ui(opts.rank, opts.rank, 1);
    }
  } else {
    algo(comb, opts.n, opts.k, opts.rank);
    print_combination(comb, opts.k);
  }

  free(comb);
  mpz_clear(opts.rank);
  mpz_clear(binom);

  return 0;
}
