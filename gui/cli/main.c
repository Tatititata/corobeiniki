#include "../../collision_simulator/specification.h"
#include "../../collision_simulator/t_e_t_r_i_s/game_logic.h"
#include "input.h"
#include "renderer.h"

int main() {
  init_ncurses();

  InputEvent_t action = Pause;
  bool hold = false;
  StateInfo_t game;

  while (action != Terminate) {
    game = game_state_updating();
    render_game(&game);
    get_user_input(&action, &hold, game.pause);
    userInput(action, hold);
  }
  end_ncurses();
  return 0;
}
