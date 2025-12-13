#include <stdio.h>

// Function to swap two numbers using XOR
void swap(int *a, int *b) {
    if (*a != *b) {  // XOR swap fails if both are same memory location
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

int main() {
    int x, y;
    
    printf("Enter first number: ");
    scanf("%d", &x);
    
    printf("Enter second number: ");
    scanf("%d", &y);
    
    printf("\nBefore swapping:\n");
    printf("x = %d, y = %d\n", x, y);
    
    swap(&x, &y);
    
    printf("\nAfter swapping:\n");
    printf("x = %d, y = %d\n", x, y);
    
    return 0;
}