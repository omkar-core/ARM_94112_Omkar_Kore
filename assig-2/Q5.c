#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t reg = 0xAA;  // 1010 1010
    
    printf("Original register: 0x%02X\n\n", reg);
    
    // Task 1: Read bits 2 to 4 (3 bits)
    uint8_t readValue = (reg >> 2) & 0x07;
    printf("Read bits 2-4: 0x%X (%d in decimal)\n\n", readValue, readValue);
    
    // Task 2: Write 0b011 (3) to bits 2-4
    reg = reg & ~(0x07 << 2);   // Clear bits 2-4
    reg = reg | (0x03 << 2);    // Set new value
    
    printf("After writing 0b011 to bits 2-4: 0x%02X\n", reg);
    
    return 0;
}