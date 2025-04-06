#include "specification.h"

#include "tetris/game_logic.h"

void userInput(UserAction_t action, bool hold) {
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

GameInfo_t updateCurrentState() {
  Extend *game = get_game();
  update_game(game);
  return game->game;
}
