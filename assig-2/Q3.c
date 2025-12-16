#include <stdio.h>
#include <stdint.h>

int main() {
    uint16_t reg = 0xABCD;
    
    // Extract lower 4 bits (bits 3-0)
    uint8_t lower4 = reg & 0x0F;
    
    // Extract upper 4 bits of lower byte (bits 7-4)
    uint8_t upper4_lower_byte = (reg >> 4) & 0x0F;
    
    printf("Original value: 0x%04X\n\n", reg);
    printf("Lower 4 bits:                0x%X\n", lower4);
    printf("Upper 4 bits of lower byte:  0x%X\n", upper4_lower_byte);
    
    return 0;
}