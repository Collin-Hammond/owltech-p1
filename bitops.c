#include "bitops.h"
#include <stdint.h>

unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value | (1u << position);
}

unsigned int ClearBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value & ~(1u << position);
}

unsigned int ToggleBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value ^ (1u << position);
}

int GetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return 0;
    return (value >> position) & 1u;
}

int CountBits(unsigned int value) {
    int cnt = 0;
    while (value) {
        value &= (value - 1u);
        cnt++;
    }
    return cnt;
}

unsigned int ShiftLeft(unsigned int value, int positions) {
    if (positions < 0) return value;
    if (positions >= 32) return 0u;
    return value << positions;
}

unsigned int ShiftRight(unsigned int value, int positions) {
    if (positions < 0) return value;
    if (positions >= 32) return 0u;
    return value >> positions;
}

void PrintBinary(unsigned int value) {
    for (int i = 31; i >= 0; --i) {
        putchar((value >> i) & 1u ? '1' : '0');
        if (i % 4 == 0 && i != 0) putchar(' ');
    }
    putchar('\n');
}

void PrintHex(unsigned int value) {
    printf("0x%08X\n", value);
}

void Phase1_SelfTest(void) {
    puts("=== Phase 1 self-test ===");
    unsigned int v = 0;
    v = SetBit(v, 3);
    printf("SetBit 3 -> "); PrintBinary(v);
    v = ToggleBit(v, 3);
    printf("ToggleBit 3 -> "); PrintBinary(v);
    v = SetBit(v, 31);
    printf("SetBit 31 -> "); PrintBinary(v);
    printf("#ones: %d (expect 1)\n", CountBits(v));
    printf("ShiftLeft(1,4) -> "); PrintBinary(ShiftLeft(1,4));
    printf("ShiftRight(16,4) -> "); PrintBinary(ShiftRight(16,4));
    printf("Hex test: "); PrintHex(0xDEADBEEF);
    puts("=========================");
}
