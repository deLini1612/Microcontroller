/*
Lib for ESP32C3 Base Address
See page 90 Technical Reference for details
*/

#define C3_SYSTEM               0x600c0000
#define C3_SENSITIVE            0x600c1000
#define C3_INTERRUPT            0x600c2000
#define C3_EXTMEM               0x600c4000
#define C3_MMU_TABLE            0x600c5000
#define C3_AES                  0x6003a000
#define C3_SHA                  0x6003b000
#define C3_RSA                  0x6003c000
#define C3_HMAC                 0x6003e000
#define C3_DIGITAL_SIGNATURE    0x6003d000
#define C3_GDMA                 0x6003f000
#define C3_ASSIST_DEBUG         0x600ce000
#define C3_DEDICATED_GPIO       0x600cf000
#define C3_WORLD_CNTL           0x600d0000
#define C3_UART                 0x60000000
#define C3_SPI1                 0x60002000
#define C3_SPI0                 0x60003000
#define C3_GPIO                 0x60004000
#define C3_FE2                  0x60005000
#define C3_FE                   0x60006000
#define C3_RTCCNTL              0x60008000
#define C3_IO_MUX               0x60009000
#define C3_RTC_I2C              0x6000e000
#define C3_UART1                0x60010000
#define C3_I2C_EXT              0x60013000
#define C3_UHCI0                0x60014000
#define C3_RMT                  0x60016000
#define C3_LEDC                 0x60019000
#define C3_EFUSE                0x60008800
#define C3_NRX                  0x6001CC00
#define C3_BB                   0x6001D000
#define C3_TIMERGROUP0          0x6001F000
#define C3_TIMERGROUP1          0x60020000
#define C3_SYSTIMER             0x60023000
#define C3_SPI2                 0x60024000
#define C3_SYSCON               0x60026000
#define C3_APB_CTRL             0x60026000
#define C3_TWAI                 0x6002B000
#define C3_I2S0                 0x6002D000
#define C3_APB_SARADC           0x60040000
#define C3_USB_SERIAL_JTAG      0x60043000
#define C3_AES_XTS              0x600CC000
