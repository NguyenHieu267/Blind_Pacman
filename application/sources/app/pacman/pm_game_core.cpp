#include "pm_game_core.h"

Character pacman, blinky, pinky, inky, clyde;
int frightened_timer = 0; 
int game_time_left = 40; 
int time_played = 0;
uint16_t anim_tick = 0;
static uint16_t tick_counter = 0;

void pm_game_core_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case PM_GAME_SETUP: {
        APP_DBG_SIG("PM_GAME_SETUP\n");
        pm_maze_init();
        pm_pacman_init();
        pm_ghost_init();
        frightened_timer = 0; 
        time_played = 0; 
        tick_counter = 0;
        anim_tick = 0;
        game_time_left = set_time_limit;
    } break;

    case PM_GAME_TICK: {
        anim_tick++;
        tick_counter++;
        
        // Cập nhật đếm ngược
        if (tick_counter % 8 == 0) {
            time_played++;
            if (set_time_limit > 0) {
                game_time_left--;
                if (game_time_left <= 0) { 
                    task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_LOSE); 
                    return; 
                }
            }
        }

        if (frightened_timer > 0) frightened_timer--;

        // Xử lý Pacman
        pm_pacman_update_move();
        int eat_status = pm_maze_eat_dot(pacman.x, pacman.y);
        if (eat_status == 2) frightened_timer = 60; // Ăn cherry

        // Kiểm tra Win
        if (dots_left == 0) { 
            pm_eeprom_update_ranking(time_played); 
            task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_WIN); 
            return; 
        }

        // Báo cho màn hình vẽ lại
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_PACMAN_GAME_UPDATE);
    } break;

    case PM_GAME_DIR_UP: {
        pm_pacman_change_dir(DIR_UP);
    } break;

    case PM_GAME_DIR_DOWN: {
        pm_pacman_change_dir(DIR_DOWN);
    } break;

    case PM_GAME_DIR_LEFT: {
        pm_pacman_change_dir(DIR_LEFT);
    } break;

    case PM_GAME_DIR_RIGHT: {
        pm_pacman_change_dir(DIR_RIGHT);
    } break;

    default:
        break;
    }
}