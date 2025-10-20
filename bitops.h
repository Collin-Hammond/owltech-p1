#ifndef BITOPS_H
#define BITOPS_H

#include <stdio.h>

unsigned int SetBit(unsigned int value, int position);
unsigned int ClearBit(unsigned int value, int position);
unsigned int ToggleBit(unsigned int value, int position);
int GetBit(unsigned int value, int position);
int CountBits(unsigned int value);
unsigned int ShiftLeft(unsigned int value, int positions);
unsigned int ShiftRight(unsigned int value, int positions);
void PrintBinary(unsigned int value);
void PrintHex(unsigned int value);

/* Small self-test you can call from main if desired */
void Phase1_SelfTest(void);

#endif // BITOPS_H
