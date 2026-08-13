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
    view_render.clear(); // Xóa nền đen

    // 1. Tính toán cuộn camera theo tọa độ Y của Pacman
    int16_t scroll_y = (pacman.y * 8) - 28; 
    if (scroll_y < 0) scroll_y = 0;
    if (scroll_y > (32 * 8 - 64)) scroll_y = (32 * 8 - 64);

    // 2. Tính toán tầm nhìn (Blind Mode)
    int vision = (set_difficulty == 0) ? 99 : (set_difficulty == 1 ? 5 : 2);

    // 3. Vẽ Bản Đồ
    for (int r = 0; r < 32; r++) {
        for (int c = 0; c < 16; c++) {
            if (abs(r - pacman.y) + abs(c - pacman.x) <= vision) {
                int dx = c * 8; int dy = (r * 8) - scroll_y;
                if (dy >= -8 && dy <= 64) {
                    if (game_maze[r][c] == 1) view_render.drawBitmap(dx, dy, bitmap_wall_8x8, 8, 8, WHITE);
                    else if (game_maze[r][c] == 0) view_render.fillRect(dx + 3, dy + 3, 2, 2, WHITE); 
                    else if (game_maze[r][c] == 3) view_render.drawLine(dx, dy + 4, dx + 7, dy + 4, WHITE);
                    else if (game_maze[r][c] == 4) view_render.drawBitmap(dx, dy, bitmap_cherry, 8, 8, WHITE);
                }
            }
        }
    }

    const unsigned char* g_bmp_normal = (anim_tick % 2) ? bitmap_ghost_2legs : bitmap_ghost_3legs;
    const unsigned char* g_bmp = (frightened_timer > 0) ? bitmap_ghost_scared : g_bmp_normal;

    if (abs(blinky.y - pacman.y) + abs(blinky.x - pacman.x) <= vision) view_render.drawBitmap(blinky.x * 8, (blinky.y * 8) - scroll_y, g_bmp, 8, 8, WHITE);
    if (abs(pinky.y - pacman.y) + abs(pinky.x - pacman.x) <= vision)  view_render.drawBitmap(pinky.x * 8,  (pinky.y * 8) - scroll_y, g_bmp, 8, 8, WHITE);
    if (abs(inky.y - pacman.y) + abs(inky.x - pacman.x) <= vision)   view_render.drawBitmap(inky.x * 8,   (inky.y * 8) - scroll_y, g_bmp, 8, 8, WHITE);
    if (abs(clyde.y - pacman.y) + abs(clyde.x - pacman.x) <= vision)  view_render.drawBitmap(clyde.x * 8,  (clyde.y * 8) - scroll_y, g_bmp, 8, 8, WHITE);

    // 5. Vẽ Pacman
    view_render.drawBitmap(pacman.x * 8, (pacman.y * 8) - scroll_y, (anim_tick%2)?bitmap_pacman_open:bitmap_pacman_closed, 8, 8, WHITE);

    // 6. Vẽ thời gian
    if (set_time_limit > 0) {
        view_render.fillRect(2, 2, 24, 10, 0); 
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(4, 3);
        view_render.print(game_time_left); view_render.print("s");
    }
}

void pm_game_screen_handle(ak_msg_t *msg) {
    ak_msg_t core_msg;

    switch (msg->sig) {
    case SCREEN_ENTRY:
        core_msg.sig = PM_GAME_SETUP;
        pm_game_core_handle(&core_msg);
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, 120, TIMER_PERIODIC);
        break;

    case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK: 
        core_msg.sig = PM_GAME_TICK;
        pm_game_core_handle(&core_msg);
        break;

    case AC_DISPLAY_BUTON_DOWN_PRESSED: 
        core_msg.sig = PM_GAME_DIR_LEFT; 
        pm_game_core_handle(&core_msg);
        break;

    case AC_DISPLAY_BUTON_UP_PRESSED: 
        core_msg.sig = PM_GAME_DIR_UP; 
        pm_game_core_handle(&core_msg);
        break;

    case AC_DISPLAY_PACMAN_GAME_UPDATE:
        break;
    
    case AC_DISPLAY_SHOW_WIN:
        view_render.clear();
        view_render.setTextSize(2);
        view_render.setTextColor(WHITE);
        view_render.setCursor(20, 20);
        view_render.print("YOU WIN");
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        break;

    case AC_DISPLAY_SHOW_LOSE:
        view_render.clear();
        view_render.setTextSize(2);
        view_render.setTextColor(WHITE);
        view_render.setCursor(18, 20);
        view_render.print("YOU LOSE");
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu); 
        break;
    }
}