#include "renderer.h"

WINDOW *get_tetris() {
  static WINDOW *tetris = NULL;
  if (!tetris) tetris = newwin(H + 2, (W * CELL_WIDTH) + 2, 0, 0);
  return tetris;
}

WINDOW *get_next() {
  static WINDOW *next = NULL;
  if (!next) next = newwin(H + 2, W + 2, 0, (W * CELL_WIDTH) + 3);
  return next;
}

WINDOW *get_controls() {
  static WINDOW *controls = NULL;
  if (!controls) {
    controls = newwin(H + 2, W + 2, 0, W * (CELL_WIDTH + 1) + 6);
    box(controls, 0, 0);
    print_controls(controls);
  }
  return controls;
}

void render_game(const GameInfo_t *game) {
  WINDOW *tetris = get_tetris();
  WINDOW *next = get_next();
  refresh();
  box(tetris, 0, 0);
  box(next, 0, 0);
  print_next_screen(next, game->level, game->score, game->high_score);
  if (game->pause) {
    print_pause_screen(tetris, next, game->pause, game->score,
                       game->high_score);
  } else {
    print_playing_field(tetris, game->field, H, W, 1, 1);
    print_playing_field(next, game->next, 4, 4, 3, 2);
  }
  wrefresh(tetris);
  wrefresh(next);
  wrefresh(get_controls());
}

void init_colors() {
  start_color();
  init_pair(4, COLOR_CYAN, COLOR_CYAN);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
  init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
}

void init_ncurses() {
  initscr();
  noecho();
  srand(time(0));
  keypad(stdscr, true);
  curs_set(0);
  init_colors();
  start_color();
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  init_color(COLOR_YELLOW, 900, 900, 200);
  init_color(COLOR_GREEN, 100, 600, 100);
  init_color(COLOR_RED, 900, 100, 100);
  init_color(COLOR_BLUE, 0, 300, 1000);
  init_color(COLOR_CYAN, 1000, 500, 0);
}

void clear_playing_field(WINDOW *win, uint8_t h, uint8_t w, uint8_t dy,
                         uint8_t dx) {
  for (uint8_t y = dy; y < h + dy; y++) {
    for (uint8_t x = dx; x < w + dx; x++) {
      mvwaddch(win, y, x, ' ');
    }
  }
}

void print_pause_screen(WINDOW *tetris, WINDOW *next, int pause, int score,
                        int high_score) {
  clear_playing_field(tetris, H, W * CELL_WIDTH, 1, 1);
  clear_playing_field(next, 4, 4 * CELL_WIDTH, 3, 2);
  if (pause == 1) {
    mvwprintw(tetris, 9, 2, "Pause");
    mvwprintw(tetris, 11, 2, "'p' to resume");
  } else if (pause == 2) {
    mvwprintw(tetris, 9, 2, "Press");
    mvwprintw(tetris, 11, 2, "Enter ");
    mvwprintw(tetris, 13, 2, "to start");
  } else if (pause == 3) {
    mvwprintw(tetris, 6, 3, "Game Over");
    mvwprintw(tetris, 9, 3, "Your");
    mvwprintw(tetris, 10, 3, "score: %d", score);
    mvwprintw(tetris, 12, 3, "High");
    mvwprintw(tetris, 13, 3, "score: %d", high_score);
    mvwprintw(tetris, 17, 2, "Press Enter");
    mvwprintw(tetris, 18, 2, "to start new game");
  }
}

void print_next_screen(WINDOW *next, int level, int score, int high_score) {
  mvwprintw(next, 1, 3, "Next:");
  mvwprintw(next, 8, 3, "Level:");
  mvwprintw(next, 11, 3, "Score:");
  mvwprintw(next, 14, 3, "High");
  mvwprintw(next, 15, 3, "score:");
  mvwprintw(next, 9, 3, "%d", level);
  mvwprintw(next, 12, 3, "%d", score);
  mvwprintw(next, 16, 3, "%d", high_score);
}

void print_playing_field(WINDOW *win, int **field, uint8_t h, uint8_t w,
                         uint8_t shift_y, uint8_t shift_x) {
  for (uint8_t y = 0; y < h; y++) {
    for (uint8_t x = 0; x < w; x++) {
      if (!field[y][x]) {
        for (uint8_t dx = 0; dx < CELL_WIDTH; dx++) {
          mvwaddch(win, y + shift_y, (x * CELL_WIDTH) + shift_x + dx, ' ');
        }
      } else {
        wattron(win, COLOR_PAIR(field[y][x]));
        for (int dx = 0; dx < CELL_WIDTH; dx++) {
          mvwaddch(win, y + shift_y, (x * CELL_WIDTH) + shift_x + dx, ' ');
        }
        wattroff(win, COLOR_PAIR(field[y][x]));
      }
    }
  }
}

void print_controls(WINDOW *controls) {
  mvwaddch(controls, 2, 2, ACS_LARROW);
  mvwprintw(controls, 2, 4, "move");
  mvwprintw(controls, 3, 4, "left");
  mvwaddch(controls, 5, 2, ACS_RARROW);
  mvwprintw(controls, 5, 4, "move");
  mvwprintw(controls, 6, 4, "right");
  mvwaddch(controls, 8, 2, ACS_UARROW);
  mvwprintw(controls, 8, 4, "rotate");
  mvwaddch(controls, 10, 2, ACS_DARROW);
  mvwprintw(controls, 10, 4, "drop");
  mvwprintw(controls, 17, 1, "'q' quit");
  mvwprintw(controls, 19, 1, "'p' pause");
}

void end_ncurses() {
  WINDOW *tetris = get_tetris();
  WINDOW *next = get_next();
  WINDOW *controls = get_controls();
  delwin(tetris);
  delwin(next);
  delwin(controls);
  curs_set(1);
  echo();
  endwin();
}
