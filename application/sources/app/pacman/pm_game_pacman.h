#ifndef __PM_GAME_PACMAN_H__
#define __PM_GAME_PACMAN_H__

#include <stdint.h>
#include <stdbool.h>

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

#endif // __PM_GAME_PACMAN_H__