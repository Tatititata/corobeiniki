#ifndef TETRIS_TESTS
#define TETRIS_TESTS

#include <check.h>

#include "../collision_simulator/specification.h"
#include "../collision_simulator/cubix/game_logic.h"
#include "../gui/cli/renderer.h"

Suite *cubix();
Suite *specification();
#endif