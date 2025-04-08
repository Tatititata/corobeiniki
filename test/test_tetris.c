#include "test.h"

START_TEST(test_initialize_game) {
  Extend *game = get_game();
  initialize_game(game);
  start_game();
  ck_assert_ptr_nonnull(game->game.field);
  ck_assert_ptr_nonnull(game->game.next);
  ck_assert_int_eq(game->state, MOVING);
  ck_assert_int_ge(game->game.high_score, 0);
  terminate_game();
}
END_TEST

START_TEST(test_move_left) {
  Extend *game = get_game();
  initialize_game(game);
  game->x = 5;
  game->state = MOVING;
  move_left();
  ck_assert_int_eq(game->x, 4);
  terminate_game();
}
END_TEST

START_TEST(test_move_right) {
  Extend *game = get_game();
  initialize_game(game);
  game->x = 5;
  game->state = MOVING;
  move_right();
  ck_assert_int_eq(game->x, 6);
  terminate_game();
}
END_TEST

START_TEST(test_rotate) {
  Extend *game = get_game();
  initialize_game(game);
  game->current = 0;
  game->rot = 0;
  rotate();
  ck_assert_int_eq(game->rot, 1);
  rotate();
  ck_assert_int_eq(game->rot, 2);
  rotate();
  ck_assert_int_eq(game->rot, 3);
  terminate_game();
}
END_TEST

START_TEST(test_drop) {
  Extend *game = get_game();
  initialize_game(game);
  game->y = 0;
  game->state = MOVING;
  drop();
  ck_assert_int_eq(game->state, SPAWN);
  terminate_game();
}
END_TEST

START_TEST(test_check_lines) {
  Extend *game = get_game();
  initialize_game(game);
  start_game();
  for (int x = 1; x <= W; x++) {
    game->arr[x] = 1;
  }
  check_lines(game);
  ck_assert_int_gt(game->game.score, 0);
  terminate_game();
}
END_TEST

START_TEST(test_block_fits) {
  Extend *game = get_game();
  initialize_game(game);
  start_game();
  game->current = 0;
  int8_t fits =
      block_fits(game->arr, game->current, game->y, game->x, game->rot);
  ck_assert_int_eq(fits, 1);
  terminate_game();
}
END_TEST

Suite *t_e_t_r_i_s() {
  Suite *s = suite_create("s21_t_e_t_r_i_s tests");
  TCase *tc_core = tcase_create("Tetris");
  tcase_add_test(tc_core, test_initialize_game);
  tcase_add_test(tc_core, test_move_left);
  tcase_add_test(tc_core, test_move_right);
  tcase_add_test(tc_core, test_rotate);
  tcase_add_test(tc_core, test_drop);
  tcase_add_test(tc_core, test_check_lines);
  tcase_add_test(tc_core, test_block_fits);

  suite_add_tcase(s, tc_core);
  return s;
}