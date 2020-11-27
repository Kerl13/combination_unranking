#include "../combunrank.h"


const name_algo_pair unrank_algo_list[] = {
  {.name = "recursive_method_naive", .func = unrank_recursive_method_naive},
  {.name = "recursive_method_tr", .func = unrank_recursive_method_tr},
  {.name = "recursive_method", .func = unrank_recursive_method},
  {.name = "factoradics_naive", .func = unrank_factoradics_naive},
  {.name = "factoradics", .func = unrank_factoradics},
  {.name = "combinadics_naive", .func = unrank_combinadics_naive},
  {.name = "combinadics", .func = unrank_combinadics},
  {.name = "combinadics2_naive", .func = unrank_combinadics2_naive},
  {.name = "combinadics2", .func = unrank_combinadics2},
};
