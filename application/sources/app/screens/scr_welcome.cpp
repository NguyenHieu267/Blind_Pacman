#include "scr_welcome.h"

static uint8_t welcome_pac_x = 0; 
static uint16_t welcome_anim_tick = 0;

static void view_scr_welcome();

view_dynamic_t dyn_view_welcome = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_welcome
};

view_screen_t scr_welcome = {
	&dyn_view_welcome,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_welcome(){
	view_render.clear();
	// PACMAN
	view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(28, 15);
    view_render.print("PACMAN");

	// Ghost chase Pacman
	int p1_x = welcome_pac_x;			// Pacman 
    int g1_x = welcome_pac_x - 16;		// Ghost is 16 pixels far from Pacman 
	for (int dx = 0; dx < 128; dx += 12){      // Draw bean
    	if(dx > p1_x + 6){
            view_render.fillRect(dx, 35, 2, 2, WHITE);
        }
    }
	view_render.drawBitmap(
        p1_x, 32, 
        (welcome_anim_tick%2)?bitmap_pacman_open:bitmap_pacman_closed,      // Draw bitmap pacman open + closed
        8, 
        8, 
        WHITE
    );
    view_render.drawBitmap(
        g1_x, 32, 
        (welcome_anim_tick%2)?bitmap_ghost_2legs:bitmap_ghost_3legs,        // Draw ghost running
        8, 
        8, 
        WHITE
    );
	
	// Press MODE ->
	view_render.setTextSize(1);
    view_render.setCursor(18, 45);
    view_render.print("Press MODE ->");
}

void scr_welcome_handle(ak_msg_t *msg){
	switch (msg->sig){
	case SCREEN_ENTRY:{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		welcome_pac_x = 0;
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, 120, TIMER_PERIODIC);
	} break;

	case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK:{
        welcome_anim_tick++;
        welcome_pac_x += 4;
        if (welcome_pac_x >= 144) welcome_pac_x -= 144; 

    } break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:{ 
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu);    // Move to scr_menu
    } break;

    default: break;
    }
}
