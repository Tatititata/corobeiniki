#include "../../brick_game/specification.h"
#include "../../brick_game/tetris/game_logic.h"
#include "input.h"
#include "renderer.h"

int main() {
  init_ncurses();

  UserAction_t action = Pause;
  bool hold = false;
  GameInfo_t game;

  while (action != Terminate) {
    game = updateCurrentState();
    render_game(&game);
    get_user_input(&action, &hold, game.pause);
    userInput(action, hold);
  }
  end_ncurses();
  return 0;
}
