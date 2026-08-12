#include "pm_game_screen.h"

static void view_pm_game_screen();

view_dynamic_t dyn_view_pm_game_screen = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_pm_game_screen
};

view_screen_t pm_game_screen = {
	&dyn_view_pm_game_screen,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_pm_game_screen() {
    view_render.clear();
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(15, 25);
    view_render.print("PLAYING...");
}

void pm_game_screen_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY:
        // Gửi Event SETUP sang Core Game xử lý
        task_post_pure_msg(AC_TASK_DISPLAY_ID, PM_GAME_SETUP);
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, 120, TIMER_PERIODIC);
        break;

    case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK: 
        // Bắn Event TICK sang Core để cập nhật logic
        task_post_pure_msg(AC_TASK_DISPLAY_ID, PM_GAME_TICK);
        break;

    case AC_DISPLAY_BUTON_DOWN_PRESSED: 
        task_post_pure_msg(AC_TASK_DISPLAY_ID, PM_GAME_DIR_LEFT); 
        break;

    case AC_DISPLAY_BUTON_UP_PRESSED: 
        task_post_pure_msg(AC_TASK_DISPLAY_ID, PM_GAME_DIR_UP); 
        break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu); 
        break;
    }
}