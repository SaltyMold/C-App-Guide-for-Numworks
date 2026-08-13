#include "../libs/eadk.h"

extern void _eadk_keyboard_scan_do_scan(void);
extern uint32_t _eadk_keyboard_scan_low(void);
extern uint32_t _eadk_keyboard_scan_high(void);

eadk_keyboard_state_t eadk_keyboard_scan(void) {
  _eadk_keyboard_scan_do_scan();
  uint64_t low = (uint64_t)_eadk_keyboard_scan_low();
  uint64_t high = (uint64_t)_eadk_keyboard_scan_high();
  return (high << 32) | low;
}

extern uint32_t _eadk_timing_millis_low(void);
extern uint32_t _eadk_timing_millis_high(void);

uint64_t eadk_timing_millis(void) {
  uint64_t low = (uint64_t)_eadk_timing_millis_low();
  uint64_t high = (uint64_t)_eadk_timing_millis_high();
  return (high << 32) | low;
}