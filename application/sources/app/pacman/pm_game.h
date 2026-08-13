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

// pm_game_eeprom
extern difficulty_t set_difficulty;
extern sound_state_t set_sound;
extern uint8_t set_time_limit;
extern uint32_t top_times[3];

extern void pm_eeprom_save_settings(void);
extern void pm_eeprom_update_ranking(uint32_t time);

// pm_game_screen
extern view_screen_t pm_game_screen;
extern void pm_game_screen_handle(ak_msg_t* msg);

#endif // __PM_GAME_H__