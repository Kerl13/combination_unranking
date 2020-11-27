#include "../combunrank.h"


const name_algo_pair unrank_algo_list_typo[] = {
  {.name = "recursive_method", .func = unrank_recursive_method},
  {.name = "recursive_method_tr", .func = unrank_recursive_method_tr},
  {.name = "recursive_method_naive", .func = unrank_recursive_method_naive},
  {.name = "factoradics", .func = unrank_factoradics},
  {.name = "factoradics_naive", .func = unrank_factoradics_naive},
  {.name = "combinadics_naive", .func = unrank_combinadics_naive},
  {.name = "combinadics_naive2", .func = unrank_combinadics_naive2},
};
