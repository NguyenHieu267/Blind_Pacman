#ifndef PM_GAME_MAZE_H
#define PM_GAME_MAZE_H

#include "pm_game.h"

#define MAZE_WIDTH  16
#define MAZE_HEIGHT 32

//============== MAZE RULE ==============
// 0: Empty / Dot
// 1: Wall
// 2: Ghost way
// 3: Ghost area
// 4: Cherry
enum { 
    MAZE_DOT,
    MAZE_WALL,
    MAZE_GHOST,
    MAZE_GHOST_AREA,
    MAZE_CHERRY,
    MAZE_EMPTY
};

extern uint8_t game_maze[MAZE_HEIGHT][MAZE_WIDTH];
extern int dots_left;

// Init maze
void pm_maze_init();

// Check wall
bool pm_maze_is_wall_pacman(int x, int y);
bool pm_maze_is_wall_ghost(int x, int y);

// Eat dot / cherry
int pm_maze_eat_dot(int x, int y);

// Draw maze
void pm_maze_render(void);

#endif // __PM_GAME_MAZE_H__