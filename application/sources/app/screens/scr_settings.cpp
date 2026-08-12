#include "scr_settings.h"

static uint8_t set_cursor = 0;
uint8_t set_difficulty = 0; 
uint8_t set_sound_on   = 1; 
uint8_t set_time_limit = 40;

void view_scr_settings();

view_dynamic_t dyn_view_settings = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_settings
};

view_screen_t scr_settings = {
	&dyn_view_settings,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_settings(){
    view_render.clear();
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(20, 0); view_render.print("--- SETTINGS ---");
    
    view_render.setCursor(0, 15); view_render.print(set_cursor==0 ? "> DIFF: " : "  DIFF: ");
    if (set_difficulty == 0) view_render.print("EASY");
    else if (set_difficulty == 1) view_render.print("MEDIUM");
    else view_render.print("HARD");

    view_render.setCursor(0, 27); view_render.print(set_cursor==1 ? "> SOUND: " : "  SOUND: ");
    view_render.print(set_sound_on ? "ON" : "OFF");

    view_render.setCursor(0, 39); view_render.print(set_cursor==2 ? "> LIMIT: " : "  LIMIT: ");
    if (set_time_limit == 0) view_render.print("NO");
    else { view_render.print(set_time_limit); view_render.print("s"); }

    view_render.setCursor(0, 51); view_render.print(set_cursor==3 ? "> BACK TO MENU" : "  BACK TO MENU");
}

void scr_settings_handle(ak_msg_t *msg){
    switch (msg->sig){
    case SCREEN_ENTRY:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT); // Auto sleep 20s
        break;
    case AC_DISPLAY_SHOW_IDLE:
        SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        break;
    case AC_DISPLAY_BUTON_DOWN_PRESSED: // Trái / Đi Xuống
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT);
        set_cursor = (set_cursor < 3) ? set_cursor + 1 : 0;
        break;
    case AC_DISPLAY_BUTON_UP_PRESSED: // Giữa / Đi Lên
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT);
        set_cursor = (set_cursor > 0) ? set_cursor - 1 : 3;
        break;
    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        switch(set_cursor){
            case 0:
            set_difficulty = (set_difficulty < 2) ? set_difficulty + 1 : 0;
            break;

            case 1:
            set_sound_on = !set_sound_on;
            break;

            case 2: 
            if(set_time_limit == 0) set_time_limit = 10;
            else{ 
                set_time_limit += 10; 
                if (set_time_limit > 150) set_time_limit = 0;
            }
            break;

            case 3:
            SCREEN_TRAN(scr_menu_handle, &scr_menu);
            break;
        }
            default: break;
    }
}