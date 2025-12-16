#include <stdio.h>
#include <stdint.h>

// Function to print binary value (8-bit)
void printBinary(uint8_t value)
{
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}

int main(void)
{
    uint8_t status = 0x08;     // 00001000
    uint8_t bitToCheck = 3;
    uint8_t mask;
    uint8_t result;

    printf("CHECK A SPECIFIC BIT USING AND OPERATION\n\n");

    // Display status value
    printf("Status (Hex)    : 0x%02X\n", status);
    printf("Status (Binary) : ");
    printBinary(status);
    printf("\n\n");

    // Create mask
    mask = (1 << bitToCheck);
    printf("Mask for bit %d : ", bitToCheck);
    printBinary(mask);
    printf(" (1 << %d)\n\n", bitToCheck);

    // AND operation
    result = status & mask;

    printf("Result of AND   : ");
    printBinary(result);
    printf("\n\n");

    // Decision
    if (result != 0)
    {
        printf("Decision: Bit %d is SET\n", bitToCheck);
    }
    else
    {
        printf("Decision: Bit %d is CLEAR\n", bitToCheck);
    }

    return 0;
}
