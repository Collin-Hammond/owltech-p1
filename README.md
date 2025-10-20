# CS 3503 Project 1 - BitBoard Checkers 

## Author
Collin Hammond

## Files
- `bitops.h` / `bitops.c` — Phase 1 bit manipulation functions and small self-test
- `game.h` / `game.c` — Phase 2 game state and helpers (bitboards)
- `main.c` — interactive CLI and glue (calls Phase1 self-test)
- `Makefile` — build rules
- `sample_output.txt` — example run output

## Build
```bash
make
```
## Run
```bash
./checkers
```
## Game
Commands
from to - move a piece
save name - save the current game
load name - load a saved game
quit - exit the game