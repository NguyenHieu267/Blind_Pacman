#ifndef __PM_GAME_CORE_H__
#define __PM_GAME_CORE_H__

#include <stdint.h>
#include <stdbool.h>

// Định nghĩa hướng
typedef enum { DIR_IDLE = 0, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction_t;

struct Character { int x, y; direction_t dir; };

// Biến cài đặt hệ thống
extern uint8_t set_difficulty; 
extern uint8_t set_sound_on; 
extern uint8_t set_time_limit; 
extern uint32_t top_times[3];

// Biến trạng thái game
extern Character pacman, blinky, pinky, inky, clyde;
extern uint8_t game_maze[32][16];
extern int frightened_timer;
extern int dots_left;
extern int game_time_left;
extern int time_played;

// Hàm chức năng
void pm_load_system_data();
void pm_save_settings();
void pm_update_leaderboard(uint32_t time);
void pm_init_game();
void pm_change_dir(direction_t new_dir);
int pm_update_tick(uint16_t anim_tick); // Trả về 0: Đang chơi, 1: Win, 2: Lose

#endif // __PM_GAME_CORE_H__