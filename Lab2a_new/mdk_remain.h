#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regBase.h"



enum { GPIO_OUT_EN = 8, GPIO_OUT_FUNC = 341, GPIO_IN_FUNC = 85, GPIO_CONFIG_REG = 29};

// Perform `count` "NOP" operations
static inline void spin(volatile unsigned long count) {
  while (count--) asm volatile("nop");
}

static inline uint64_t systick(void) {
  REG(C3_SYSTIMER)[1] = BIT(30);  // TRM 10.5
  spin(1);
  return ((uint64_t) REG(C3_SYSTIMER)[16] << 32) | REG(C3_SYSTIMER)[17];
}

static inline uint64_t uptime_us(void) {
  return systick() >> 4;
}

static inline void delay_us(unsigned long us) {
  uint64_t until = uptime_us() + us;
  while (uptime_us() < until) spin(1);
}

static inline void delay_ms(unsigned long ms) {
  delay_us(ms * 1000);
}

static inline void wdt_disable(void) {
  REG(C3_RTCCNTL)[42] = 0x50d83aa1;  // Disable write protection
  // REG(C3_RTCCNTL)[36] &= BIT(31);    // Disable RTC WDT
  REG(C3_RTCCNTL)[36] = 0;  // Disable RTC WDT
  REG(C3_RTCCNTL)[35] = 0;  // Disable

  // bootloader_super_wdt_auto_feed()
  REG(C3_RTCCNTL)[44] = 0x8F1D312A;
  REG(C3_RTCCNTL)[43] |= BIT(31);
  REG(C3_RTCCNTL)[45] = 0;

  REG(C3_TIMERGROUP0)[63] &= ~BIT(9);  // TIMG_REGCLK -> disable TIMG_WDT_CLK
  REG(C3_TIMERGROUP0)[18] = 0;         // Disable TG0 WDT
  REG(C3_TIMERGROUP1)[18] = 0;         // Disable TG1 WDT

  printf("wdt_disable successfully... ");
}

static inline void wifi_get_mac_addr(uint8_t mac[6]) {
  uint32_t a = REG(C3_EFUSE)[17], b = REG(C3_EFUSE)[18];
  mac[0] = (b >> 8) & 255, mac[1] = b & 255, mac[2] = (uint8_t) (a >> 24) & 255;
  mac[3] = (a >> 16) & 255, mac[4] = (a >> 8) & 255, mac[5] = a & 255;
}

static inline void soc_init(void) {
  // Init clock. TRM 6.2.4.1
  REG(C3_SYSTEM)[2] &= ~3U;
  REG(C3_SYSTEM)[2] |= BIT(0) | BIT(2);
  REG(C3_SYSTEM)[22] = BIT(19) | (40U << 12) | BIT(10);
  // REG(C3_RTCCNTL)[47] = 0; // RTC_APB_FREQ_REG -> freq >> 12
  ((void (*)(int)) 0x40000588)(160);  // ets_update_cpu_frequency(160)

#if 0
  // Configure system clock timer, TRM 8.3.1, 8.9
  REG(C3_TIMERGROUP0)[1] = REG(C3_TIMERGROUP0)[2] = 0UL;  // Reset LO and HI
  REG(C3_TIMERGROUP0)[8] = 0;                             // Trigger reload
  REG(C3_TIMERGROUP0)[0] = (83U << 13) | BIT(12) | BIT(29) | BIT(30) | BIT(31);
#endif
}
