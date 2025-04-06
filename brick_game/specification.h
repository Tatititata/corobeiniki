#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W 10                     ///< Width of the Tetris playing field.
#define H 20                     ///< Height of the Tetris playing field.
#define V 20                     ///< Initial Speed of the Tetris game.
#define _POSIX_C_SOURCE 199309L  // Для nanosleep

/**
 * @brief Enumeration of user actions.
 */
typedef enum {
  Start,      ///< Start a new game.
  Pause,      ///< Pause/resume the game.
  Terminate,  ///< Terminate the game.
  Left,       ///< Move the current piece left.
  Right,      ///< Move the current piece right.
  Up,         ///< Rotate the current piece.
  Down,       ///< Drop the current.
  Action      ///< No action.
} UserAction_t;

/**
 * @brief Structure containing game information.
 */
typedef struct {
  int **field;     ///< 2D array representing the Tetris playing field.
  int **next;      ///< 2D array representing the next piece to be displayed.
  int score;       ///< Current score of the game.
  int high_score;  ///< Highest score achieved in the game.
  int level;       ///< Current level of the game.
  int speed;       ///< Game speed (determines how quickly pieces fall).
  int pause;       ///< Pause state (0 = not paused, !0 = paused).
} GameInfo_t;

/**
 * @brief Handles user input.
 * @param action The user action to perform.
 * @param hold A boolean indicating whether the key is being held down.
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Updates the current game state.
 * @return The updated GameInfo_t structure.
 */
GameInfo_t updateCurrentState();

#endif
