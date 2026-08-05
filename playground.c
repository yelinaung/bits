#include <stdio.h>
#include <stdint.h>

/* Exercise 1: print the 8 bits of `value`, most significant bit first.
 *
 * Hints:
 *   - Loop from position 7 down to 0.
 *   - For each position, you need to answer one question:
 *     "is the bit at that position a 1 or a 0?"
 *   - You already know how to build a number with one bit set at position n.
 *   - putchar('0') and putchar('1') print a single character.
 */
void print_bits(uint8_t value) {
    /* your code here */
}

int main(void) {
    /* Predict each answer BEFORE you run this. */
    printf("45      = "); print_bits(45);     printf("\n");
    printf("1 << 6  = "); print_bits(1 << 6); printf("\n");
    printf("3 << 4  = "); print_bits(3 << 4); printf("\n");
    printf("200 >> 3= "); print_bits(200 >> 3); printf("\n");
    return 0;
}
