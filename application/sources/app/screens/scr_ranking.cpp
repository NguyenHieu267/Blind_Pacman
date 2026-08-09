#include "scr_ranking.h"
#include "scr_menu.h"
#include "scr_welcome.h"
#include "pm_game_core.h"
#include "timer.h"

static void view_scr_ranking();
view_dynamic_t dyn_view_ranking = { { .item_type = ITEM_TYPE_DYNAMIC, }, view_scr_ranking };
view_screen_t scr_ranking = { &dyn_view_ranking, ITEM_NULL, ITEM_NULL, .focus_item = 0 };

static void view_scr_ranking() {
    view_render.clear();
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(35, 0); view_render.print("RANKING");
    for (int i = 0; i < 3; i++) {
        view_render.setCursor(20, 20 + i*12);
        view_render.print("TOP "); view_render.print(i+1); view_render.print(": ");
        if (top_times[i] == 999) view_render.print("-- s");
        else { view_render.print(top_times[i]); view_render.print(" s"); }
    }
    view_render.setCursor(15, 55); view_render.print("Press MODE to exit");
}

void scr_ranking_handle(ak_msg_t *msg) {
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