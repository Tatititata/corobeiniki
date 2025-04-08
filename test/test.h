#ifndef TETRIS_TESTS
#define TETRIS_TESTS

#include <check.h>

#include "../collision_simulator/specification.h"
#include "../collision_simulator/t_e_t_r_i_s/game_logic.h"
#include "../gui/cli/renderer.h"

Suite *t_e_t_r_i_s();
Suite *specification();
#endif