#include <stdio.h>
#include <stdlib.h>  // For atoi()

// Recursive function to print binary
void printBinary(int n) {
    if (n > 1) {
        printBinary(n / 2);
    }
    printf("%d", n % 2);
}

int main(int argc, char *argv[]) {
    int num;
    
    // Check if argument is provided
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        printf("Example: %s 13\n", argv[0]);
        return 1;
    }
    
    // Convert string argument to integer
    num = atoi(argv[1]);
    
    printf("Binary of %d = ", num);
    
    if (num == 0) {
        printf("0");
    } else {
        printBinary(num);
    }
    
    printf("\n");
    
    return 0;
}