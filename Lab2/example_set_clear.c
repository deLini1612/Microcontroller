// Example of set and clear nth bit of GPIO_ENABLE_REG
#include <bits/stdint-uintn.h>

#define MASK(n) ((uint32_t) 1U << (n))
#define GPIO_ENABLE_REG 0x60004020
#define REG(x) (*((volatile uint32_t *) (x)))

uint32_t set_nth(unsigned n){
    REG(GPIO_ENABLE_REG) |= MASK(n);
}
uint32_t clear_nth(int n){
    REG(GPIO_ENABLE_REG) &= ~MASK(n);
}