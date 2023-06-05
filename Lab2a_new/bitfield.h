#include <stdint.h>

#define BIT(bit) ((uint32_t)1U << (bit))
#define REG(address) ((volatile uint32_t *)(address))
#define GET_BIT(address, bit) ((*REG(address) & BIT(bit)) ? 1 : 0)
#define SET_BIT(address, bit) (*REG(address) |= BIT(bit))
#define CLEAR_BIT(address, bit) (*REG(address) &= ~BIT(bit))
#define TOGGLE_BIT(address, bit) (*REG(address) ^= BIT(bit))
#define FIELD(lsb, msb) ((uint32_t)(BIT(msb + 1) - BIT(lsb)))
#define CLEAR_FIELD(address, lsb, msb)(*REG(address) &= ~FIELD(lsb, msb))
#define WRITE_FIELD(address, lsb, msb, value) ({ CLEAR_FIELD(address, lsb, msb); \
                                                *REG(address) |= (uint32_t) (value << (lsb)) & FIELD(lsb, msb); })
                                                
