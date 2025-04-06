#ifndef TETRIS_TESTS
#define TETRIS_TESTS

#include <check.h>

#include "../brick_game/specification.h"
#include "../brick_game/tetris/game_logic.h"
#include "../gui/cli/renderer.h"

Suite *tetris();
Suite *specification();
#endif