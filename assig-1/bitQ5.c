#include <stdio.h>

// Function to check if character is alphabet
int isAlphabet(char ch) {
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        return 1;  // Is alphabet
    }
    return 0;  // Not alphabet
}

// Function to print binary (8 bits)
void printBinary(char ch) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (ch >> i) & 1);
    }
}

// Function to check alphabet and perform XOR
void checkAndXOR(char ch) {
    printf("Input character: '%c'\n", ch);
    printf("ASCII value: %d\n", ch);
    printf("Binary: ");
    printBinary(ch);
    printf("\n\n");
    
    if (isAlphabet(ch)) {
        printf("Result: It IS an alphabet!\n\n");
        
        // Perform XOR with 32
        char result = ch ^ 32;
        
        printf("Performing XOR with 32:\n");
        printf("------------------------\n");
        printf("  ");
        printBinary(ch);
        printf(" ('%c' = %d)\n", ch, ch);
        printf("^ ");
        printBinary(32);
        printf(" (32)\n");
        printf("------------------------\n");
        printf("= ");
        printBinary(result);
        printf(" ('%c' = %d)\n", result, result);
        printf("\n");
        
        printf("Observation: '%c' changed to '%c'\n", ch, result);
        
        if (ch >= 'A' && ch <= 'Z') {
            printf("UPPERCASE -> lowercase\n");
        } else {
            printf("lowercase -> UPPERCASE\n");
        }
    } else {
        printf("Result: It is NOT an alphabet!\n");
    }
}

int main() {
    char ch;
    
    printf("Enter a character: ");
    scanf("%c", &ch);
    
    printf("\n");
    checkAndXOR(ch);
    
    return 0;
}