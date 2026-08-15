#include "pm_game_screen.h"

#define PIXELS_PER_BLOCK 8         // 8 pixels -> 1 block
#define CAMERA_OFFSET_Y  28        // Value to center Pacman on the lcd (Scrolling to pacman)

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

static int get_vision_radius() {            // Handle Settings DIFF
    if (set_difficulty == DIFF_EASY) return 99;
    if (set_difficulty == DIFF_MEDIUM) return 5;
    return 2; // DIFF_HARD
}

static void draw_maze_layer(int16_t scroll_y, int vision) {
    for (int r = 0; r < MAZE_HEIGHT; r++) {
        for (int c = 0; c < MAZE_WIDTH; c++) {
            if (abs(r - pacman.y) + abs(c - pacman.x) <= vision) {
                int dx = c * PIXELS_PER_BLOCK; 
                int dy = (r * PIXELS_PER_BLOCK) - scroll_y;
                
                if (dy >= -PIXELS_PER_BLOCK && dy <= LCD_HEIGHT) {
                    switch (game_maze[r][c]) {
                        case MAZE_WALL:
                            view_render.drawBitmap(dx, dy, bitmap_wall_8x8, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, WHITE);
                            break;
                        
                        case MAZE_DOT:
                            view_render.fillRect(dx + 3, dy + 3, 2, 2, WHITE);
                            break;

                        case MAZE_GHOST:
                            view_render.drawLine(dx, dy + 4, dx + 7, dy + 4, WHITE);
                            break;

                        case MAZE_CHERRY:
                            view_render.drawBitmap(dx, dy, bitmap_cherry, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, WHITE);
                            break;
                    }
                }
            }
        }
    }
}

static void draw_ghost(Character g, int16_t scroll_y, int vision, const unsigned char* bitmap) {
    if (abs(g.y - pacman.y) + abs(g.x - pacman.x) <= vision) {
        view_render.drawBitmap(g.x * PIXELS_PER_BLOCK, g.y * PIXELS_PER_BLOCK - scroll_y, bitmap, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, WHITE);
    }
}

static void draw_characters_layer(int16_t scroll_y, int vision) {
    const unsigned char* g_bmp_normal = (anim_tick % 2) ? bitmap_ghost_2legs : bitmap_ghost_3legs;
    const unsigned char* g_bmp = (frightened_timer > 0) ? bitmap_ghost_scared : g_bmp_normal;

    draw_ghost(blinky, scroll_y, vision, g_bmp);
    draw_ghost(pinky, scroll_y, vision, g_bmp);
    draw_ghost(inky, scroll_y, vision, g_bmp);
    draw_ghost(clyde, scroll_y, vision, g_bmp);

    const unsigned char* p_bmp = (anim_tick % 2) ? bitmap_pacman_open : bitmap_pacman_closed;
    view_render.drawBitmap(pacman.x * PIXELS_PER_BLOCK, (pacman.y * PIXELS_PER_BLOCK) - scroll_y, p_bmp, PIXELS_PER_BLOCK, PIXELS_PER_BLOCK, WHITE);
}

static void draw_ui_layer() {
    if (set_time_limit > 0) {
        view_render.fillRect(2, 2, 24, 10, 0); 
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(4, 3);
        view_render.print(game_time_left); 
        view_render.print("s");
    }
}

void view_pm_game_screen() {
    view_render.clear(); 

    int16_t scroll_y = (pacman.y * PIXELS_PER_BLOCK) - CAMERA_OFFSET_Y; 
    if (scroll_y < 0) scroll_y = 0;
    if (scroll_y > (MAZE_HEIGHT * PIXELS_PER_BLOCK - LCD_HEIGHT)) scroll_y = (MAZE_HEIGHT * PIXELS_PER_BLOCK - LCD_HEIGHT);

    int vision = get_vision_radius();

    draw_maze_layer(scroll_y, vision);
    draw_characters_layer(scroll_y, vision);
    draw_ui_layer();
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
        core_msg.sig = PM_GAME_BUTTON_DOWN; 
        pm_game_core_handle(&core_msg);
        break;

    case AC_DISPLAY_BUTON_UP_PRESSED: 
        core_msg.sig = PM_GAME_BUTTON_UP; 
        pm_game_core_handle(&core_msg);
        break;
    
    case AC_DISPLAY_SHOW_WIN:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_game_win_handle, &scr_game_win);
        break;

    case AC_DISPLAY_SHOW_LOSE:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
        break;

    case AC_DISPLAY_BUTON_MODE_PRESSED:
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        SCREEN_TRAN(scr_menu_handle, &scr_menu); 
        break;
    }
}