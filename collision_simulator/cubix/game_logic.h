#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <string.h>

#include "../specification.h"

/**
 * @brief Enumeration of game states.
 */
typedef enum {
  START,    ///< Game is starting.
  MOVING,   ///< Piece is currently moving.
  COLLIDE,  ///< Piece has collided with the bottom or another piece.
  SPAWN,    ///< Spawn a new piece.
  GAMEOVER  ///< Game over state.
} Game_state_t;

/**
 * @brief Structure containing extended game information.
 */
typedef struct {
  uint8_t arr[(W + 2) *
              (H + 1)];  ///< Array representing the extended playing field.
  uint8_t current;       ///< The current Tetromino piece.
  int8_t y;              ///< The y-coordinate of the current block.
  int8_t x;              ///< The x-coordinate of the current block.
  uint8_t rot;           ///< The rotation of the current block.
  uint8_t next;          ///< The next block.
  Game_state_t state;    ///< The current game state.
  StateInfo_t game;      ///< The StateInfo_t structure.
} Extend;

/**
 * @brief Gets the game state.
 * @return A pointer to the Extend structure containing the game state.
 */
Extend *get_game();

/**
 * @brief Updates the game state.
 * @param game A pointer to the Extend structure containing the game state.
 */
void update_game(Extend *game);

/**
 * @brief Initializes the game.
 * @param game A pointer to the Extend structure containing the game state.
 */
void initialize_game(Extend *game);

/**
 * @brief Draws the new playing field.
 * @param game A pointer to the Extend structure containing the game state.
 */
void draw_new_field(Extend *game);

/**
 * @brief Fills the playing field.
 * @param game A pointer to the Extend structure containing the game state.
 */
void fill_field(Extend *game);

/**
 * @brief Initializes a matrix (2D array).
 * @param matrix A pointer to the matrix to initialize.
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @return 0 on failure, 1 on success.
 */
int initialize_matrix(int ***matrix, int rows, int cols);

/**
 * @brief Calculates the rotation of a block.
 * @param dy The y-coordinate delta.
 * @param dx The x-coordinate delta.
 * @param rot The rotation value.
 * @return The rotated coordinate.
 */
uint16_t rotation(uint16_t dy, uint16_t dx, uint16_t rot);

/**
 * @brief Runs the finite state machine.
 * @param game A pointer to the Extend structure containing the game state.
 */
void run_game(Extend *game);

/**
 * @brief Gets a block from a block.
 * @param block_num The block number.
 * @param dy The y-coordinate of the block.
 * @param dx The x-coordinate of the block.
 * @param rot The rotation of the block.
 * @return The block value.
 */
uint8_t block(uint8_t block_num, int dy, int dx, uint8_t rot);

/**
 * @brief Checks if a block fits in the playing field.
 * @param arr A pointer to the playing field array.
 * @param current The current block.
 * @param y The y-coordinate of the block.
 * @param x The x-coordinate of the block.
 * @param rot The rotation of the block.
 * @return True if the block fits, false otherwise.
 */
uint8_t block_fits(const uint8_t *arr, uint8_t current, int8_t y, int8_t x,
                   uint8_t rot);

/**
 * @brief Checks for completed lines in the playing field.
 * @param game A pointer to the Extend structure containing the game state.
 */
void check_lines(Extend *game);

/**
 * @brief Starts the game.
 */
void start_game();

/**
 * @brief Pauses the game.
 */
void pause_game();

/**
 * @brief Terminates the game.
 */
void terminate_game();

/**
 * @brief Moves the current piece to the left.
 */
void move_left();

/**
 * @brief Moves the current piece to the right.
 */
void move_right();

/**
 * @brief Rotates the current piece.
 */
void rotate();

/**
 * @brief Moves the current piece down.
 * @param game A pointer to the Extend structure containing the game state.
 * @return 1 if the piece moved down, 0 if it collided.
 */
int8_t move_down(Extend *game);

/**
 * @brief Drops the current piece to the bottom.
 */
void drop();

/**
 * @brief Waits for a specified number of frames.
 * @param n The number of frames to wait.
 */
void stay_still(uint8_t n);

/**
 * @brief Sets the next block.
 * @param game A pointer to the Extend structure containing the game state.
 */
void set_next(Extend *game);

/**
 * @brief Writes high score to file.
 * @param game A pointer to the Extend structure containing the game state.
 */
void save_high_score(const Extend *game);

/**
 * @brief Increases score and level.
 * @param game A pointer to the Extend structure containing the game state.
 * @param lines The number of lines cleared in a game.
 */
void count_score(Extend *game, uint8_t lines);

/**
 * @brief Increases score and level.
 * @param game A pointer to the Extend structure containing the game state.
 * @param dy The offset from which the memory is moved and the line is cleared.
 */
void clear_and_shift_line(Extend *game, uint8_t dy);

/**
 * @brief It Locks the block into place, checks for cleared lines, and prepares
 * the game for the next block.
 * @param game A pointer to the Extend structure containing the game state.
 */
void proceed_collide(Extend *game);

/**
 * @brief Handles the logic for spawning a new block at the top of the game
 * board.
 * @param game A pointer to the Extend structure containing the game state.
 */
void proceed_spawn(Extend *game);

#endif
