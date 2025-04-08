#include "input.h"

void get_user_input(InputEvent_t *action, bool *hold, int state) {
  int c = ERR;

  if (state) {
    nodelay(stdscr, FALSE);  // delay
    c = getch();
    if (c == 'p' && state == 1) {
      *action = Pause;
    } else if (c == 10 && (state == 2 || state == 3)) {
      *action = Start;
    } else if (c == 'q') {
      *action = Terminate;
    } else
      *action = Action;
  } else {
    nodelay(stdscr, TRUE);  // nodelay
    c = getch();
    if (c == KEY_UP) {
      if (!(*hold)) {
        *action = Up;
        *hold = true;
      } else {
        *action = Action;
      }
    } else {
      *hold = false;
      if (c == KEY_DOWN) {
        *action = Down;
      } else if (c == KEY_RIGHT) {
        *action = Right;
      } else if (c == KEY_LEFT) {
        *action = Left;
      } else if (c == 'p') {
        *action = Pause;
      } else if (c == 'q') {
        *action = Terminate;
      } else
        *action = Action;
    }
  }
}