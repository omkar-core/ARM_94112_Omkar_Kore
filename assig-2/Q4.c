#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t value = 5;
    
    printf("Original value: %d\n\n", value);
    
    // Left shift by 2 positions
    uint8_t leftShifted = value << 2;
    printf("Left shift by 2:  %d << 2 = %d\n", value, leftShifted);
    
    // Right shift by 1 position
    uint8_t rightShifted = value >> 1;
    printf("Right shift by 1: %d >> 1 = %d\n", value, rightShifted);
    
    return 0;
}