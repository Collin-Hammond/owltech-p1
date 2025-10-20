#include <stdio.h>
#include <string.h>
#include "bitops.h"
#include "game.h"



int main(void) {
    Phase1_SelfTest();

    GameState g;
    init_standard(&g);

    printf("OwlTech Bitboard Checkers (prototype)\n");
    printf("Commands:\n  from to    e.g. '12 21'\n  save name\n  load name\n  quit\n\n");

    char line[256];
    while (1) {
        print_board(&g);
        int win = check_win(&g);
        if (win) {
            if (win == 1) printf("Black wins!\n"); else printf("Red wins!\n");
            break;
        }
        printf("Player %s > ", g.turn == 0 ? "Red" : "Black");
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0] == '\n') continue;
        if (strncmp(line, "quit", 4) == 0) break;
        if (strncmp(line, "save", 4) == 0) {
            char fname[128];
            if (sscanf(line+4, "%127s", fname) == 1) {
                if (save_game(&g, fname)) printf("Saved to %s\n", fname);
                else printf("Save failed\n");
            } else printf("Usage: save filename\n");
            continue;
        }
        if (strncmp(line, "load", 4) == 0) {
            char fname[128];
            if (sscanf(line+4, "%127s", fname) == 1) {
                if (load_game(&g, fname)) printf("Loaded %s\n", fname);
                else printf("Load failed\n");
            } else printf("Usage: load filename\n");
            continue;
        }
        int from, to;
        if (sscanf(line, "%d %d", &from, &to) != 2) {
            printf("Invalid input. Use 'from to' or save/load/quit.\n");
            continue;
        }
        int cap = is_legal_capture_and_capture_idx(&g, from, to);
        if (cap >= 0) {
            if (!apply_move(&g, from, to, cap)) { printf("Failed move\n"); continue; }
            // chaining: naive auto-chaining
            while (1) {
                int found = 0;
                for (int t = 0; t < 64; ++t) {
                    int c2 = is_legal_capture_and_capture_idx(&g, to, t);
                    if (c2 >= 0) { apply_move(&g, to, t, c2); to = t; found = 1; break; }
                }
                if (!found) break;
            }
            g.turn = 1 - g.turn;
            continue;
        }
        if (is_legal_simple_move(&g, from, to)) {
            apply_move(&g, from, to, -1);
            g.turn = 1 - g.turn;
            continue;
        }
        printf("Illegal move\n");
    }

    printf("Goodbye\n");
    return 0;
}
