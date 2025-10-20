#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline U64 bitmask(int pos) { return (pos >= 0 && pos < 64) ? (1ULL << pos) : 0ULL; }
static inline int popcount_u64(U64 x) { return __builtin_popcountll(x); }

const U64 FILE_A = 0x0101010101010101ULL;
const U64 FILE_H = 0x8080808080808080ULL;

void init_standard(GameState *g) {
    g->red = g->red_k = g->black = g->black_k = 0ULL;
    // Place on dark squares ((r+f)%2 == 1)
    for (int r = 0; r < 3; ++r) {
        for (int f = 0; f < 8; ++f) {
            if ((r + f) % 2 == 1) g->red |= bitmask(r*8 + f);
        }
    }
    for (int r = 5; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            if ((r + f) % 2 == 1) g->black |= bitmask(r*8 + f);
        }
    }
    g->turn = 0;
}

U64 all_pieces(const GameState *g) {
    return g->red | g->red_k | g->black | g->black_k;
}

void print_board(const GameState *g) {
    printf("Board (format: piece index). r/R = red man/king, b/B = black man/king\n");
    for (int r = 7; r >= 0; --r) {
        for (int f = 0; f < 8; ++f) {
            int idx = r*8 + f;
            U64 m = bitmask(idx);
            char ch = '.';
            if (g->red & m) ch = 'r';
            if (g->red_k & m) ch = 'R';
            if (g->black & m) ch = 'b';
            if (g->black_k & m) ch = 'B';
            printf("%c%02d ", ch, idx);
        }
        printf("\n");
    }
    printf("Turn: %s\n", g->turn == 0 ? "Red (r/R)" : "Black (b/B)");
}

int occupied(const GameState *g, int pos) {
    return ((all_pieces(g) >> pos) & 1ULL) != 0;
}

int is_legal_simple_move(const GameState *g, int from, int to) {
    if (from < 0 || from >= 64 || to < 0 || to >= 64) return 0;
    U64 fm = bitmask(from), tm = bitmask(to);
    if ((all_pieces(g) & tm) != 0) return 0;
    int player = g->turn;
    U64 men = player == 0 ? g->red : g->black;
    U64 kings = player == 0 ? g->red_k : g->black_k;
    if ((men & fm) == 0 && (kings & fm) == 0) return 0;
    int diff = to - from;
    if ((men & fm) != 0) {
        if (player == 0) {
            if (diff == 9 && ((FILE_A & fm) == 0)) return 1;
            if (diff == 7 && ((FILE_H & fm) == 0)) return 1;
        } else {
            if (diff == -7 && ((FILE_A & fm) == 0)) return 1;
            if (diff == -9 && ((FILE_H & fm) == 0)) return 1;
        }
    } else {
        if (diff == 9 && ((FILE_A & fm) == 0)) return 1;
        if (diff == 7 && ((FILE_H & fm) == 0)) return 1;
        if (diff == -7 && ((FILE_A & fm) == 0)) return 1;
        if (diff == -9 && ((FILE_H & fm) == 0)) return 1;
    }
    return 0;
}

int is_legal_capture_and_capture_idx(const GameState *g, int from, int to) {
    if (from < 0 || from >= 64 || to < 0 || to >= 64) return -1;
    if (occupied(g, to)) return -1;
    int player = g->turn;
    U64 opp = (player == 0) ? (g->black | g->black_k) : (g->red | g->red_k);
    U64 men = player == 0 ? g->red : g->black;
    // U64 kings = player == 0 ? g->red_k : g->black_k;
    int diff = to - from;
    if ((men & bitmask(from)) != 0) {
        if (player == 0) {
            if (diff == 18) { int cap = from + 9; if ((opp >> cap) & 1ULL) return cap; }
            if (diff == 14) { int cap = from + 7; if ((opp >> cap) & 1ULL) return cap; }
        } else {
            if (diff == -14) { int cap = from - 7; if ((opp >> cap) & 1ULL) return cap; }
            if (diff == -18) { int cap = from - 9; if ((opp >> cap) & 1ULL) return cap; }
        }
    } else {
        if (diff == 18) { int cap = from + 9; if ((opp >> cap) & 1ULL) return cap; }
        if (diff == 14) { int cap = from + 7; if ((opp >> cap) & 1ULL) return cap; }
        if (diff == -14) { int cap = from - 7; if ((opp >> cap) & 1ULL) return cap; }
        if (diff == -18) { int cap = from - 9; if ((opp >> cap) & 1ULL) return cap; }
    }
    return -1;
}

int apply_move(GameState *g, int from, int to, int capture_idx) {
    if (from < 0 || from >= 64 || to < 0 || to >= 64) return 0;
    U64 fm = bitmask(from), tm = bitmask(to);
    int player = g->turn;
    U64 *men = (player == 0) ? &g->red : &g->black;
    U64 *kings = (player == 0) ? &g->red_k : &g->black_k;
    int moving_is_king = ((*kings) & fm) != 0;
    if (((*men) & fm) == 0 && !moving_is_king) return 0;
    (*men) &= ~fm; (*kings) &= ~fm;
    if (moving_is_king) *kings |= tm; else *men |= tm;
    if (capture_idx >= 0) {
        U64 cm = bitmask(capture_idx);
        if (player == 0) { g->black &= ~cm; g->black_k &= ~cm; }
        else { g->red &= ~cm; g->red_k &= ~cm; }
    }
    int rank = to / 8;
    if (!moving_is_king) {
        if (player == 0 && rank == 7) { g->red &= ~tm; g->red_k |= tm; }
        if (player == 1 && rank == 0) { g->black &= ~tm; g->black_k |= tm; }
    }
    return 1;
}

int check_win(const GameState *g) {
    int rc = popcount_u64(g->red | g->red_k);
    int bc = popcount_u64(g->black | g->black_k);
    if (rc == 0) return 1;
    if (bc == 0) return 2;
    return 0;
}

/* Save format:
   red:0x...
   red_k:0x...
   black:0x...
   black_k:0x...
   turn:0 or 1
*/
int save_game(const GameState *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fprintf(f, "red:0x%016llX\n", (unsigned long long)g->red);
    fprintf(f, "red_k:0x%016llX\n", (unsigned long long)g->red_k);
    fprintf(f, "black:0x%016llX\n", (unsigned long long)g->black);
    fprintf(f, "black_k:0x%016llX\n", (unsigned long long)g->black_k);
    fprintf(f, "turn:%d\n", g->turn);
    fclose(f);
    return 1;
}

int load_game(GameState *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    char key[64];
    unsigned long long val;
    for (int i = 0; i < 5; ++i) {
        if (fscanf(f, "%63[^:]:%llx\n", key, &val) != 2) { fclose(f); return 0; }
        if (strcmp(key, "red") == 0) g->red = (U64)val;
        else if (strcmp(key, "red_k") == 0) g->red_k = (U64)val;
        else if (strcmp(key, "black") == 0) g->black = (U64)val;
        else if (strcmp(key, "black_k") == 0) g->black_k = (U64)val;
        else if (strcmp(key, "turn") == 0) g->turn = (int)val;
    }
    fclose(f);
    return 1;
}

