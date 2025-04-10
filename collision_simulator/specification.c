#include "specification.h"

#include "t_e_t_r_i_s/game_logic.h"

void userInput(InputEvent_t action, bool hold) {
  if (action == Start) {
    start_game();
  } else if (action == Pause) {
    pause_game();
  } else {
    if (action == Terminate) {
      terminate_game();
    } else if (action == Left) {
      move_left();
    } else if (action == Right) {
      move_right();
    } else if (action == Up && hold) {
      rotate();
    } else if (action == Down) {
      drop();
    }
  }
}

StateInfo_t game_state_updating() {
  Extend *game = get_game();
  update_game(game);
  return game->game;
}
