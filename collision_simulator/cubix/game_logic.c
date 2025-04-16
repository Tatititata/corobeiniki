#include "game_logic.h"

Extend *get_game() {
  static Extend game = {0};
  return &game;
}

void update_game(Extend *game) {
  if (!game->game.field) {
    initialize_game(game);
    if (!game->state) game->game.pause = 2;
  } else {
    run_game(game);
    fill_field(game);
  }
}

void run_game(Extend *game) {
  static int tick_counter = 0;
  stay_still(game->game.speed - game->game.level);
  tick_counter++;

  if (game->state == MOVING) {
    if (tick_counter >= game->game.speed - game->game.level) {
      tick_counter = 0;
      int8_t res = move_down(game);
      if (!res) game->state = COLLIDE;
    }
  } else if (game->state == COLLIDE) {
    proceed_collide(game);
  } else if (game->state == SPAWN) {
    proceed_spawn(game);
  }
  if (game->state == GAMEOVER) {
    save_high_score(game);
    game->game.pause = 3;
  }
}

void proceed_collide(Extend *game) {
  for (int dy = 0; dy < 4; dy++) {
    for (int dx = 0; dx < 4; dx++) {
      if (block(game->current, dy, dx, game->rot))
        game->arr[(game->y + dy) * (W + 2) + game->x + dx] = game->current + 1;
    }
  }
  check_lines(game);
  game->state = SPAWN;
}

void proceed_spawn(Extend *game) {
  game->current = game->next;
  set_next(game);
  int8_t res =
      block_fits(game->arr, game->current, game->y, game->x, game->rot);
  if (!res)
    game->state = GAMEOVER;
  else
    game->state = MOVING;
}

void check_lines(Extend *game) {
  uint8_t lines = 0;

  for (uint8_t dy = 0; dy < 4; dy++) {
    if (game->y + dy < H) {
      uint32_t line = 1;
      for (uint8_t dx = 1; dx <= W && line; dx++)
        line *= game->arr[(game->y + dy) * (W + 2) + dx];
      if (line) {
        lines++;
        clear_and_shift_line(game, dy);
      }
    }
  }
  if (lines) count_score(game, lines);
}

void clear_and_shift_line(Extend *game, uint8_t dy) {
  uint8_t size = (game->y + dy) * (W + 2);
  memmove(&(game->arr[W + 2]), &(game->arr[0]), size * sizeof(uint8_t));
  memset(&(game->arr[1]), 0, W * sizeof(uint8_t));
  game->arr[0] = game->arr[W + 1] = 9;
}

void count_score(Extend *game, uint8_t lines) {
  game->game.score +=
      100 * (1 + lines * ((lines > 1) + (lines > 2)) + 6 * (lines > 3));
  uint8_t level = game->game.score / 600;
  game->game.level = (level > 10) ? 10 : level;
}

void initialize_game(Extend *game) {
  int res = initialize_matrix(&game->game.field, H, W);
  res *= initialize_matrix(&game->game.next, 4, 4);
  if (res) {
    draw_new_field(game);
  } else {
    game->state = GAMEOVER;
    terminate_game();
  }
}

void draw_new_field(Extend *game) {
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W + 2; j++) {
      game->arr[i * (W + 2) + j] = (j == 0 || j == W + 1) ? 8 : 0;
    }
  }
  for (int j = 0; j < W + 2; j++) game->arr[(H) * (W + 2) + j] = 9;

  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    if (fscanf(file, "%d", &game->game.high_score) < 0) {
      game->game.high_score = 0;
    }
    fclose(file);
  }
}

void fill_field(Extend *game) {
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      game->game.field[y][x] = game->arr[y * (W + 2) + x + 1];
    }
  }
  for (int dy = 0; dy < 4; dy++) {
    for (int dx = 0; dx < 4; dx++) {
      if (block(game->current, dy, dx, game->rot))
        game->game.field[game->y + dy][game->x + dx - 1] = game->current + 1;
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->game.next[i][j] = block(game->next, i, j, 0);
    }
  }
}

void terminate_game() {
  Extend *game = get_game();
  if (game->game.field) {
    free(game->game.field);
    game->game.field = NULL;
  }
  if (game->game.next) {
    free(game->game.next);
    game->game.next = NULL;
  }
  save_high_score(game);
}

int initialize_matrix(int ***matrix, int rows, int cols) {
  int res = 1;
  *matrix = malloc(rows * cols * sizeof(int) + rows * sizeof(int *));
  if (*matrix) {
    int *ptr = (int *)(*matrix + rows);
    for (int i = 0; i < rows; i++) {
      (*matrix)[i] = ptr + i * cols;
      memset((*matrix)[i], 0, cols * sizeof(int));
    }
  } else
    res = 0;
  return res;
}

void pause_game() {
  Extend *game = get_game();
  game->game.pause = !game->game.pause;
}

void start_game() {
  Extend *game = get_game();
  if (game->game.pause == 3) draw_new_field(game);
  game->game.pause = !game->game.pause;
  game->current = rand() % 7;
  set_next(game);
  game->game.score = 0;
  game->game.level = 0;
  game->game.speed = V;
  game->state = MOVING;
}

void move_left() {
  Extend *game = get_game();
  if (game->state == MOVING)
    game->x -=
        block_fits(game->arr, game->current, game->y, game->x - 1, game->rot);
}

void move_right() {
  Extend *game = get_game();
  if (game->state == MOVING)
    game->x +=
        block_fits(game->arr, game->current, game->y, game->x + 1, game->rot);
}

void rotate() {
  Extend *game = get_game();
  if (game->state == MOVING)
    game->rot +=
        block_fits(game->arr, game->current, game->y, game->x, game->rot + 1);
}

int8_t move_down(Extend *game) {
  int8_t fits =
      block_fits(game->arr, game->current, game->y + 1, game->x, game->rot);
  game->y += fits;
  return fits;
}

void drop() {
  Extend *game = get_game();
  while (move_down(game));
  game->state = COLLIDE;
  run_game(game);
}

uint8_t block(uint8_t block_num, int dy, int dx, uint8_t rot) {
  uint8_t blocks[7][16] = {
      {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},   // I
      {0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0},   // J
      {0, 3, 0, 0, 0, 3, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0},   // T
      {0, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0},   // Z
      {0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0},   // S
      {0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0},   // L
      {0, 0, 0, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 0, 0, 0}};  // O

  return blocks[block_num][rotation(dy, dx, rot)];
}

uint16_t rotation(uint16_t dy, uint16_t dx, uint16_t rot) {
  uint16_t turn = 0;
  rot %= 4;
  if (rot == 0)
    turn = 4 * dy + dx;
  else if (rot == 1)
    turn = 12 + dy - 4 * dx;
  else if (rot == 2)
    turn = 15 - 4 * dy - dx;
  else
    turn = 3 - dy + 4 * dx;
  return turn;
}

uint8_t block_fits(const uint8_t *arr, uint8_t current, int8_t y, int8_t x,
                   uint8_t rot) {
  uint8_t fits = 1;
  for (int8_t dy = 0; dy < 4 && fits; dy++)
    for (int8_t dx = 0; dx < 4 && fits; dx++) {
      if ((x + dx >= 0 && x + dx < W + 2) && (y + dy >= 0 && y + dy < H + 1)) {
        if (block(current, dy, dx, rot) && arr[(y + dy) * (W + 2) + x + dx])
          fits = 0;
      }
    }
  return fits;
}

void stay_still(uint8_t n) {
  clock_t start = clock();
  double wait_time = 0.0025 * n;
  while ((double)(clock() - start) / CLOCKS_PER_SEC < wait_time);
}

void set_next(Extend *game) {
  game->y = 0;
  game->x = W / 3;
  game->rot = 0;
  game->next = rand() % 7;
}

void save_high_score(const Extend *game) {
  if (game->game.score > game->game.high_score) {
    FILE *file = fopen("high_score.txt", "w");
    if (file) {
      fprintf(file, "%d", game->game.score);
      fclose(file);
    }
  }
}