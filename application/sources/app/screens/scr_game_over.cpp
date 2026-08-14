#include "scr_game_over.h"

static void view_scr_game_over();

view_dynamic_t dyn_view_game_over = { 
    { 
        .item_type = ITEM_TYPE_DYNAMIC, 
    }, 
    view_scr_game_over 
};

view_screen_t scr_game_over = { 
    &dyn_view_game_over, 
    ITEM_NULL, 
    ITEM_NULL, 
    
    .focus_item = 0 
};

void view_scr_game_over() {
    view_render.clear();

    view_render.setTextSize(2); 
    view_render.setTextColor(WHITE);
    view_render.setCursor(15, 15); view_render.print("GAME OVER");

    view_render.setTextSize(1); 
    view_render.setCursor(15, 40); view_render.print("TIME OUT");
}

void scr_game_over_handle(ak_msg_t *msg) {
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