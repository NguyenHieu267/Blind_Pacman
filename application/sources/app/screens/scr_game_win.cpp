#include "scr_game_win.h"

static void view_scr_game_win();

view_dynamic_t dyn_view_game_win = { 
    { 
        .item_type = ITEM_TYPE_DYNAMIC, 
    }, 
    view_scr_game_win 
};

view_screen_t scr_game_win = { 
    &dyn_view_game_win, 
    ITEM_NULL, 
    ITEM_NULL, 
    
    .focus_item = 0 
};

void view_scr_game_win() {
    view_render.clear();

    view_render.setTextSize(2); 
    view_render.setTextColor(WHITE);
    view_render.setCursor(20, 15); 
    view_render.print("VICTORY!");
    
    view_render.setTextSize(1); 
    view_render.setCursor(20, 40); 
    view_render.print("TIME: "); 
    view_render.print(time_played); 
    view_render.print("s");
}

void scr_game_win_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT); 
        break;

    case AC_DISPLAY_SHOW_IDLE:
        SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        break;
        
    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        SCREEN_TRAN(scr_menu_handle, &scr_menu);
        break;
    }
}