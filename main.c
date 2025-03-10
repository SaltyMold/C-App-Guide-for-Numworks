#include "eadk.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Hello World";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main() {
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, 320, 240}, eadk_color_white);

  while (true){
    eadk_display_draw_string("Hello World", (eadk_point_t){0, 0}, true, eadk_color_black, eadk_color_white);
  }

  return 0;
}