#include "scr_menu.h"

static uint8_t menu_cursor = 0; 

static void view_scr_menu();

view_dynamic_t dyn_view_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu
};

view_screen_t scr_menu = {
	&dyn_view_menu,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_menu() {
    view_render.clear();
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(30, 2); 
    view_render.print("PACMAN MENU");
    
    // Draw playing box
    view_render.drawRect(7, 16, 24, 24, (menu_cursor==0)?WHITE:0); 
    view_render.drawBitmap(11, 20, icon_play, 16, 16, WHITE);
    // Draw setting box
    view_render.drawRect(37, 16, 24, 24, (menu_cursor==1)?WHITE:0);
    view_render.drawBitmap(41, 20, setting_icon, 16, 16, WHITE);
    // Draw ranking box
    view_render.drawRect(67, 16, 24, 24, (menu_cursor==2)?WHITE:0);
    view_render.drawBitmap(71, 20, ranking_icon, 16, 16, WHITE);
    // Draw exit box
    view_render.drawRect(97, 16, 24, 24, (menu_cursor==3)?WHITE:0);
    view_render.drawBitmap(101, 20, exit_icon, 16, 16, WHITE);

    // Words at bottom
    view_render.setCursor(32, 46);
    if(menu_cursor==0) view_render.print("PLAY GAME");
    if(menu_cursor==1) view_render.print("SETTINGS");
    if(menu_cursor==2) view_render.print("RANKING");
    if(menu_cursor==3) view_render.print("EXIT / QR");
}

void scr_menu_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY: {
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 120, TIMER_PERIODIC); // Tick count for 20s to back to scr_welcome
    } break;

    case AC_DISPLAY_BUTON_DOWN_PRESSED: { 
        menu_cursor = (menu_cursor > 0) ? (menu_cursor-1) : 3;      // Move cursor to left
    } break;

    case AC_DISPLAY_BUTON_UP_PRESSED: { 
        menu_cursor = (menu_cursor < 3) ? (menu_cursor+1) : 0;      // Move cursor to right
    } break;

    case AC_DISPLAY_BUTON_MODE_PRESSED: { 
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        if (menu_cursor == 0) { 
            // SCREEN_TRAN(scr_pacman_game_handle, &scr_pacman_game); 
        } else if (menu_cursor == 1) { 
            SCREEN_TRAN(scr_settings_handle, &scr_settings); 
        } else if (menu_cursor == 2) { 
            // SCREEN_TRAN(scr_ranking_handle, &scr_ranking); 
        } else if (menu_cursor == 3) { 
            SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode); 
        }
    } break;

    default: break;
    }
}