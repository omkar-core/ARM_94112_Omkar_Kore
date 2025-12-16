#include <stdio.h>
#include <stdint.h>

void printBinary(uint8_t num) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i == 4) printf(" ");
    }
}

int main() {
    uint8_t reg = 0x2A;
    
    printf("Bit positions:        7654 3210\n");
    printf("--------------------------------\n");
    
    // Initial
    printf("Initial:    0x%02X  (", reg);
    printBinary(reg);
    printf(")\n");
    
    // Set bit 4
    reg |= (1 << 4);
    printf("Set bit 4:  0x%02X  (", reg);
    printBinary(reg);
    printf(")\n");
    
    // Clear bit 1
    reg &= ~(1 << 1);
    printf("Clear bit 1: 0x%02X  (", reg);
    printBinary(reg);
    printf(")\n");
    
    // Toggle bit 5
    reg ^= (1 << 5);
    printf("Toggle bit 5: 0x%02X  (", reg);
    printBinary(reg);
    printf(")\n");
    
    return 0;
}