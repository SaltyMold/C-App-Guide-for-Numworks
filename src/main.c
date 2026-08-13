#include "libs/eadk.h"
#include "libs/storage.h"
#include "libs/macro.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Test";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

int main(void) {

    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
    eadk_display_draw_string("Press Back to quit", (eadk_point_t){0, 0}, true, eadk_color_black, eadk_color_white);

    while (1) {
        eadk_keyboard_state_t state = eadk_keyboard_scan();
        if (eadk_keyboard_key_down(state, eadk_key_back)) break;
    }

    return 0;
}