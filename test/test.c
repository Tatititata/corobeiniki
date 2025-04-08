#include "test.h"

void run_case(Suite(*test_case)) {
  SRunner *sr = srunner_create(test_case);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}

void run_test() {
  Suite *suite_cases[] = {t_e_t_r_i_s(), specification(), NULL};

  for (Suite **test_case = suite_cases; *test_case != NULL; test_case++)
    run_case(*test_case);
}

int main() {
  run_test();
  return 0;
}
