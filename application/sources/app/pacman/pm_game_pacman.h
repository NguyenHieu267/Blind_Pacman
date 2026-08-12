#ifndef __PM_GAME_PACMAN_H__
#define __PM_GAME_PACMAN_H__

#include <stdint.h>
#include <stdio.h>

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"

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

// pm_game_maze
extern view_screen_t pm_game_screen;
extern void pm_game_screen_handle(ak_msg_t* msg);

#endif // __PM_GAME_PACMAN_H__