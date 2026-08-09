#include "scr_welcome.h"
#include "scr_menu.h" 
#include "screens_bitmap.h"
#include "timer.h"
#include "sys_dbg.h"
#include "pm_game_core.h"

static int welcome_pac_x = 0; 
static uint16_t anim_tick = 0;
static uint16_t idle_timeout = 0; 

static void view_scr_welcome();

view_dynamic_t dyn_view_welcome = { { .item_type = ITEM_TYPE_DYNAMIC, }, view_scr_welcome };
view_screen_t scr_welcome = { &dyn_view_welcome, ITEM_NULL, ITEM_NULL, .focus_item = 0 };

static void view_scr_welcome() {
    view_render.fillRect(0, 0, 128, 64, 0); 
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(28, 15);
    view_render.print("PACMAN");
    view_render.setTextSize(1);
    view_render.setCursor(18, 45);
    view_render.print("Press MODE ->");
    
    // Logic hạt đậu: Ăn tới đâu mất tới đó, hết vòng reset lại
    for (int dx = 0; dx < 128; dx += 12) {
        if (dx > welcome_pac_x + 6) {
            view_render.fillRect(dx, 35, 2, 2, WHITE);
        }
    }

    view_render.drawBitmap(welcome_pac_x, 32, (anim_tick%2)?bitmap_pacman_open:bitmap_pacman_closed, 8, 8, WHITE);
    view_render.drawBitmap(welcome_pac_x - 16, 32, (anim_tick%2)?bitmap_ghost_2legs:bitmap_ghost_3legs, 8, 8, WHITE);
}

void scr_welcome_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY: {
        pm_load_system_data(); 
        welcome_pac_x = 0;
        idle_timeout = 0;
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, 120, TIMER_PERIODIC);
    } break;

    case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK: {
        anim_tick++;
        welcome_pac_x += 4;
        if (welcome_pac_x >= 144) welcome_pac_x = -24; // Đi khuất màn hình thì vòng lại
    } break;

    case AC_DISPLAY_BUTON_MODE_PRESSED: { 
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu); 
    } break;

    default: break;
    }
}