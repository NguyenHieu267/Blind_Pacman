#ifndef __PM_GAME_DEF_H__
#define __PM_GAME_DEF_H__

#include "pm_game.h"

// Define direction
typedef enum{ 
    DIR_IDLE, 
    DIR_UP, 
    DIR_DOWN, 
    DIR_LEFT, 
    DIR_RIGHT
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

enum {
    PM_GAME_SETUP,
    PM_GAME_TICK,
    PM_GAME_BUTTON_UP,
    PM_GAME_BUTTON_DOWN
};

struct Move { 
    direction_t dir; 
    int dx; 
    int dy; 
    direction_t opp; 
};

#endif //__PM_GAME_DEF_H__