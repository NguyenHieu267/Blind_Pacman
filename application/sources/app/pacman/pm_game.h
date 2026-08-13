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


// Define direction
typedef enum{ 
    DIR_IDLE, 
    DIR_UP, DIR_DOWN, 
    DIR_LEFT, DIR_RIGHT
} direction_t;

// Character (Pacman, Ghost)
struct Character { 
    int x, y; 
    direction_t dir; 
};

// Define settings Diff
typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
} difficulty_t;

// Define settings Sound
typedef enum {
    SOUND_OFF,
    SOUND_ON
} sound_state_t;

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