#include "scr_settings.h"

static setting_cursor_t set_cursor = SETTING_DIFFICULTY;

static void view_scr_settings();

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
    
    view_render.setCursor(0, 15); 
    view_render.print(set_cursor == SETTING_DIFFICULTY ? "> DIFF: " : "  DIFF: ");
        switch(set_difficulty){
            case DIFF_EASY:
                view_render.print("EASY");
            break;

            case DIFF_MEDIUM:
                view_render.print("MEDIUM");
            break;

            case DIFF_HARD:
                view_render.print("HARD");
            break;
        }

    view_render.setCursor(0, 27); 
    view_render.print(set_cursor == SETTING_SOUND ? "> SOUND: " : "  SOUND: ");
    view_render.print(set_sound == SOUND_ON ? "ON" : "OFF");

    view_render.setCursor(0, 39); 
    view_render.print(set_cursor == SETTING_LIMIT ? "> LIMIT: " : "  LIMIT: ");
        if(set_time_limit == 0) view_render.print("NO");
        else{ 
            view_render.print(set_time_limit); 
            view_render.print("s"); }

    view_render.setCursor(0, 51); view_render.print(set_cursor== SETTING_BACK ? "> BACK TO MENU" : "  BACK TO MENU");
}

void scr_settings_handle(ak_msg_t *msg){
    switch (msg->sig){
    case SCREEN_ENTRY:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT); // Back to src_welcome after 20s
        break;

    case AC_DISPLAY_SHOW_IDLE:
        SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
        break;

    case AC_DISPLAY_BUTON_DOWN_PRESSED:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT);
        set_cursor = (setting_cursor_t)((set_cursor + 1) % SETTING_COUNT);
        break;

    case AC_DISPLAY_BUTON_UP_PRESSED:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT);
        set_cursor = (set_cursor == SETTING_DIFFICULTY) ? SETTING_BACK : (setting_cursor_t)(set_cursor - 1);
        break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, 20000, TIMER_ONE_SHOT);
        switch (set_cursor){
        case SETTING_DIFFICULTY:
            set_difficulty = (set_difficulty < DIFF_HARD) ? (difficulty_t)(set_difficulty + 1) : DIFF_EASY;
            break;

        case SETTING_SOUND:
            set_sound = (set_sound == SOUND_ON) ? SOUND_OFF : SOUND_ON;
            break;

        case SETTING_LIMIT:
            if(set_time_limit == 0)
                set_time_limit = 10;
            else{
                set_time_limit += 10;
                if(set_time_limit > 150) set_time_limit = 0;
            }
            break;

        case SETTING_BACK:
            pm_eeprom_save_settings();
            SCREEN_TRAN(scr_menu_handle, &scr_menu);
            break;
        default: break;
        } 
        break;

    default: break;
    }
}