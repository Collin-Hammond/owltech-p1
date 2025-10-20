#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef uint64_t U64;

typedef struct {
    U64 red;     // red men (lowercase)
    U64 red_k;   // red kings (uppercase)
    U64 black;   // black men
    U64 black_k; // black kings
    int turn;    // 0 = red, 1 = black
} GameState;

/* Initialize starting standard position */
void init_standard(GameState *g);

/* Print board to stdout (includes bit indices) */
void print_board(const GameState *g);

/* Apply a move (from -> to) with capture index (or -1) */
int apply_move(GameState *g, int from, int to, int capture_idx);

/* Check if a simple move is legal */
int is_legal_simple_move(const GameState *g, int from, int to);

/* Check if capture move is legal and return captured index or -1 */
int is_legal_capture_and_capture_idx(const GameState *g, int from, int to);

/* Save/load state to a simple text format */
int save_game(const GameState *g, const char *filename);
int load_game(GameState *g, const char *filename);

/* Check winner: 0 none, 1 black wins, 2 red wins */
int check_win(const GameState *g);

#endif // GAME_H
