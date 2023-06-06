#include <stdint.h>

#define PLAIN_MASK(length) \
    ((length) >= 32 ? 0xFFFFFFFF : ((((uint32_t)1) << (length)) - 1))

#define FIELD_MASK(msb, lsb) \
  (PLAIN_MASK((msb) + 1) - PLAIN_MASK(lsb))

#define SET_FIELD(dest, msb, lsb, value)                        \
  (                                                           \
    (dest) = (((dest) & ~(FIELD_MASK(msb, lsb))) | \
              (((value)&PLAIN_MASK((msb) - (lsb) + 1)) << (lsb))))

#define wdt00_wdtctl (*((volatile uint32_t *)(0x80420000)))
#define WDT_KEY (28)

int main() {
    SET_FIELD(wdt00_wdtctl, 8, 4, 28); // Set field from 4th bit to 8th bit of register whose address is 0x80420000 to 28
}