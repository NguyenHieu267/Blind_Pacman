#ifndef __PM_GAME_MAZE_H__
#define __PM_GAME_MAZE_H__

#include "pm_game.h"

#define MAZE_WIDTH  16
#define MAZE_HEIGHT 32

enum {
    MAZE_DOT,
    MAZE_WALL,
    MAZE_EMPTY,
    MAZE_GHOST,
    MAZE_CHERRY
};

extern uint8_t game_maze[MAZE_HEIGHT][MAZE_WIDTH];
extern int dots_left;
extern void pm_maze_init();
extern bool pm_maze_is_wall_pacman(int x, int y);
extern bool pm_maze_is_wall_ghost(int x, int y);
extern int pm_maze_eat_dot(int x, int y);

#endif // __PM_GAME_MAZE_H__