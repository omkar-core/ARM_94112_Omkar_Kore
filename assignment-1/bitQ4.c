#include <stdio.h>

// Function to count '1' bits
int countSetBits(unsigned char byte) {
    int count = 0;
    
    while (byte > 0) {
        count += (byte & 1);
        byte = byte >> 1;
    }
    
    return count;
}

// Function to print binary (8 bits)
void printBinary(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}

// Function to check even parity and set MSB if needed
unsigned char checkAndSetParity(unsigned char byte) {
    int bitCount = countSetBits(byte);
    
    printf("Original byte: ");
    printBinary(byte);
    printf(" (%d)\n", byte);
    
    printf("Number of '1' bits: %d\n", bitCount);
    
    // Check if parity is even
    if (bitCount % 2 == 0) {
        printf("Parity: EVEN (No change needed)\n");
    } else {
        printf("Parity: ODD (Setting MSB to make it even)\n");
        byte = byte | 0x80;  // Set MSB (bit 7)
    }
    
    printf("Final byte: ");
    printBinary(byte);
    printf(" (%d)\n", byte);
    
    return byte;
}

int main() {
    unsigned char num;
    
    printf("Enter a number (0-127): ");
    scanf("%hhu", &num);
    
    printf("\n");
    checkAndSetParity(num);
    
    return 0;
}