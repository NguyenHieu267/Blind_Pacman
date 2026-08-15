#include "pm_game_pacman.h"

void pm_pacman_init() {
    pacman = {8, 21, DIR_IDLE};
}

void pm_pacman_change_dir(direction_t new_dir) {
    if (new_dir == DIR_UP || 
        new_dir == DIR_DOWN) {
        if (pacman.dir == DIR_DOWN && new_dir == DIR_UP) pacman.dir = DIR_UP;
        else if (pacman.dir == DIR_UP && new_dir == DIR_DOWN) pacman.dir = DIR_DOWN;
        else {
            if (!pm_maze_is_wall_pacman(pacman.x, pacman.y - 1)) pacman.dir = DIR_UP;
            else if (!pm_maze_is_wall_pacman(pacman.x, pacman.y + 1)) pacman.dir = DIR_DOWN;
        }
    } else {
        if (pacman.dir == DIR_RIGHT && new_dir == DIR_LEFT) pacman.dir = DIR_LEFT;
        else if (pacman.dir == DIR_LEFT && new_dir == DIR_RIGHT) pacman.dir = DIR_RIGHT;
        else {
            if (!pm_maze_is_wall_pacman(pacman.x - 1, pacman.y)) pacman.dir = DIR_LEFT;
            else if (!pm_maze_is_wall_pacman(pacman.x + 1, pacman.y)) pacman.dir = DIR_RIGHT;
        }
    }
}

void pm_pacman_update_move() {
    if (pacman.dir == DIR_UP && !pm_maze_is_wall_pacman(pacman.x, pacman.y - 1)) pacman.y--;
    else if (pacman.dir == DIR_DOWN && !pm_maze_is_wall_pacman(pacman.x, pacman.y + 1)) pacman.y++;
    else if (pacman.dir == DIR_LEFT && !pm_maze_is_wall_pacman(pacman.x - 1, pacman.y)) pacman.x--;
    else if (pacman.dir == DIR_RIGHT && !pm_maze_is_wall_pacman(pacman.x + 1, pacman.y)) pacman.x++;
    else pacman.dir = DIR_IDLE; 
}