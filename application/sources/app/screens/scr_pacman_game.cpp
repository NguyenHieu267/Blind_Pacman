#include "scr_pacman_game.h"
#include "scr_game_win.h"
#include "scr_game_over.h"
#include "scr_menu.h"
#include "pm_game_core.h"
#include "screens_bitmap.h"
#include "timer.h"

static uint16_t anim_tick = 0;
static void view_scr_pacman_game();
view_dynamic_t dyn_view_pacman_game = { { .item_type = ITEM_TYPE_DYNAMIC, }, view_scr_pacman_game };
view_screen_t scr_pacman_game = { &dyn_view_pacman_game, ITEM_NULL, ITEM_NULL, .focus_item = 0 };

static void view_scr_pacman_game() {
    view_render.clear();
    int vision = (set_difficulty == 0) ? 99 : (set_difficulty == 1 ? 5 : 2);
    int16_t scroll_y = (pacman.y * 8) - 28; 
    if (scroll_y < 0) scroll_y = 0;
    if (scroll_y > (32 * 8 - 64)) scroll_y = (32 * 8 - 64);

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

    view_render.drawBitmap(pacman.x * 8, (pacman.y * 8) - scroll_y, (anim_tick%2)?bitmap_pacman_open:bitmap_pacman_closed, 8, 8, WHITE);

    if (set_time_limit > 0) {
        view_render.fillRect(2, 2, 24, 10, 0); 
        view_render.setTextSize(1); view_render.setTextColor(WHITE);
        view_render.setCursor(4, 3);
        view_render.print(game_time_left); view_render.print("s");
    }
}

void scr_pacman_game_handle(ak_msg_t *msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY:
        pm_init_game();
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, 120, TIMER_PERIODIC);
        break;
    case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK: {
        int status = pm_update_tick(anim_tick++);
        if (status == 1) { timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK); SCREEN_TRAN(scr_game_win_handle, &scr_game_win); }
        else if (status == 2) { timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK); SCREEN_TRAN(scr_game_over_handle, &scr_game_over); }
    } break;
    case AC_DISPLAY_BUTON_DOWN_PRESSED: pm_change_dir(DIR_LEFT); break;
    case AC_DISPLAY_BUTON_UP_PRESSED: pm_change_dir(DIR_UP); break;
    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu);
        break;
    }
}