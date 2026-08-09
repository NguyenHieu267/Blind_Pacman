#include "scr_menu.h"
#include "scr_welcome.h"
#include "scr_settings.h" 
#include "scr_ranking.h"  
#include "scr_pacman_game.h" 
#include "scr_qrcode.h"
#include "screens_bitmap.h"
#include "timer.h"

static uint8_t menu_cursor = 0; 
static uint16_t idle_timeout = 0; 

static void view_scr_menu();

view_dynamic_t dyn_view_menu = { { .item_type = ITEM_TYPE_DYNAMIC, }, view_scr_menu };
view_screen_t scr_menu = { &dyn_view_menu, ITEM_NULL, ITEM_NULL, .focus_item = 0 };

static void view_scr_menu() {
    view_render.fillRect(0, 0, 128, 64, 0); 
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(30, 2); 
    view_render.print("PACMAN MENU");
    
    view_render.drawRect(7, 16, 24, 24, (menu_cursor==0)?WHITE:0); 
    view_render.drawBitmap(11, 20, icon_play, 16, 16, WHITE);

    view_render.drawRect(37, 16, 24, 24, (menu_cursor==1)?WHITE:0);
    view_render.drawBitmap(41, 20, setting_icon, 16, 16, WHITE);

    view_render.drawRect(67, 16, 24, 24, (menu_cursor==2)?WHITE:0);
    view_render.drawBitmap(71, 20, ranking_icon, 16, 16, WHITE);

    view_render.drawRect(97, 16, 24, 24, (menu_cursor==3)?WHITE:0);
    view_render.drawBitmap(101, 20, exit_icon, 16, 16, WHITE);

    view_render.setCursor(32, 46);
    if(menu_cursor==0) view_render.print("PLAY GAME");
    if(menu_cursor==1) view_render.print("SETTINGS");
    if(menu_cursor==2) view_render.print("RANKING");
    if(menu_cursor==3) view_render.print("EXIT / QR");
}

void scr_menu_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY: {
        idle_timeout = 0;
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 120, TIMER_PERIODIC); // Đếm 20s & Redraw
    } break;

    case AC_DISPLAY_SHOW_IDLE: {
        idle_timeout++;
        if (idle_timeout >= 166) { // 20s Auto-sleep
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
            SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        }
    } break;

    case AC_DISPLAY_BUTON_DOWN_PRESSED: { 
        idle_timeout = 0; 
        menu_cursor = (menu_cursor > 0) ? menu_cursor - 1 : 3; 
    } break;

    case AC_DISPLAY_BUTON_UP_PRESSED: { 
        idle_timeout = 0;
        menu_cursor = (menu_cursor < 3) ? menu_cursor + 1 : 0; 
    } break;

    case AC_DISPLAY_BUTON_MODE_PRESSED: { 
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        if (menu_cursor == 0) { 
            SCREEN_TRAN(scr_pacman_game_handle, &scr_pacman_game); // VÔ GAME
        } else if (menu_cursor == 1) { 
            SCREEN_TRAN(scr_settings_handle, &scr_settings); // VÔ SETTING
        } else if (menu_cursor == 2) { 
            SCREEN_TRAN(scr_ranking_handle, &scr_ranking); // VÔ RANKING
        } else if (menu_cursor == 3) { 
            SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode); // VÔ QR
        }
    } break;

    default: break;
    }
}