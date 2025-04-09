#ifndef RENDERER_H
#define RENDERER_H

#include "../../collision_simulator/specification.h"

#define CELL_WIDTH 2

void render_game(const StateInfo_t *game);
void init_colors();
void init_ncurses();
void end_ncurses();
void print_pause_screen(WINDOW *t_e_t_r_i_s, WINDOW *next, int pause, int score,
                        int high_score);
void print_next_screen(WINDOW *next, int level, int score, int high_score);
void print_playing_field(WINDOW *win, int **field, uint8_t h, uint8_t w,
                         uint8_t shift_y, uint8_t shift_x);
void print_controls(WINDOW *controls);
void clear_playing_field(WINDOW *win, uint8_t h, uint8_t w, uint8_t dy,
                         uint8_t dx);
void clear_next(WINDOW *next);
WINDOW *get_t_e_t_r_i_s();
WINDOW *get_next();
WINDOW *get_controls();

#endif
