#include "test.h"

START_TEST(test_user_input_start) {
  Extend *game = get_game();
  userInput(Start, false);
  ck_assert_int_eq(game->state, MOVING);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_pause) {
  Extend *game = get_game();
  userInput(Pause, false);
  ck_assert_int_eq(game->game.pause, 1);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_move_left) {
  Extend *game = get_game();
  start_game();
  game->x = 5;
  userInput(Left, false);
  ck_assert_int_eq(game->x, 4);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_move_right) {
  Extend *game = get_game();
  start_game();
  game->x = 5;
  userInput(Right, false);
  ck_assert_int_eq(game->x, 6);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_rotate) {
  Extend *game = get_game();
  start_game();
  game->current = 0;
  game->rot = 0;
  userInput(Up, true);
  ck_assert_int_eq(game->rot, 1);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_drop) {
  Extend *game = get_game();
  start_game();
  userInput(Down, false);
  ck_assert_int_eq(game->state, SPAWN);
  terminate_game();
}
END_TEST

START_TEST(test_user_input_terminate) {
  Extend *game = get_game();
  userInput(Terminate, false);
  ck_assert_ptr_null(game->game.field);
}
END_TEST

START_TEST(test_update_current_state) {
  Extend *game = get_game();
  initialize_game(game);
  StateInfo_t info = game_state_updating();
  ck_assert_ptr_nonnull(info.field);
  terminate_game();
}
END_TEST

START_TEST(test_StateInfo_t_game) {
  Extend *game = get_game();
  update_game(game);
  for (int i = 0; i < 22; i++) run_game(game);
  ck_assert_ptr_nonnull(game->game.field);
  terminate_game();
}
END_TEST

Suite *specification() {
  Suite *s = suite_create("s21_t_e_t_r_i_s spec");
  TCase *tc_core = tcase_create("Specification");
  tcase_add_test(tc_core, test_user_input_start);
  tcase_add_test(tc_core, test_user_input_pause);
  tcase_add_test(tc_core, test_user_input_move_left);
  tcase_add_test(tc_core, test_user_input_move_right);
  tcase_add_test(tc_core, test_user_input_rotate);
  tcase_add_test(tc_core, test_user_input_drop);
  tcase_add_test(tc_core, test_user_input_terminate);
  tcase_add_test(tc_core, test_update_current_state);
  tcase_add_test(tc_core, test_StateInfo_t_game);

  suite_add_tcase(s, tc_core);
  return s;
}