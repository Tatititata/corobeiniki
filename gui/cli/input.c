#include "input.h"

/* Вниманию проверяющих:

Пункт задания

  - Для управления добавь поддержку всех кнопок, предусмотренных на физической
консоли:
  ...

  - Стрелка вверх — не используется в данной игре,
  ...

  означает, что в данной игре мы не можем СДВИГАТЬ фигуру вверх (в змейке,
например, можем).

  Какой именно клавишей фигура будет вращаться, не имеет значения.
  В моей реализации за вращение отвечает стрелка вверх,
  это сделано исключительно для удобства пользователя (меня).

  Никакой принципиальной разницы не будет, если за вращение будет отвечать любая
другая клавиша.

*/

void get_user_input(UserAction_t *action, bool *hold, int state) {
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