#ifndef __PM_GAME_H__
#define __PM_GAME_H__

#include <stdint.h>
#include <stdio.h>

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include "eeprom.h"

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"

#include "pm_game_screen.h"
#include "pm_game_eeprom.h"
#include "pm_game_def.h"
#include "pm_game_maze.h"
#include "pm_game_core.h"
#include "pm_game_pacman.h"
#include "pm_game_ghost.h"

// pm_game_eeprom
extern difficulty_t set_difficulty;
extern sound_state_t set_sound;
extern uint8_t set_time_limit;
extern uint32_t top_times[3];

extern void pm_eeprom_load_data();
extern void pm_eeprom_save_settings();
extern void pm_eeprom_update_ranking(uint32_t time);

// pm_game_screen
extern view_screen_t pm_game_screen;
extern void pm_game_screen_handle(ak_msg_t* msg);

// pm_game_core
extern struct Character pacman, blinky, pinky, inky, clyde;
extern int frightened_timer;
extern int dots_left;
extern int game_time_left;
extern int time_played;
extern uint16_t anim_tick;

extern void pm_game_core_handle(ak_msg_t* msg);

// pm_game_ghost
extern void pm_ghost_init();
extern void pm_ghosts_update_move(bool is_frightened);
extern void pm_ghost_check_collision(bool is_frightened);

// pm_game_pacman
extern void pm_pacman_init();
extern void pm_pacman_toggle_horizontal(); 
extern void pm_pacman_toggle_vertical(); 
extern void pm_pacman_update_move();

#endif // __PM_GAME_H__