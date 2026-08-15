#include "pm_game_pacman.h"

void pm_pacman_init() {
    pacman = {8, 21, DIR_IDLE};
}

void pm_pacman_toggle_horizontal() {
    if (pacman.dir == DIR_LEFT) {
        pacman.dir = DIR_RIGHT;
    } 
    else if (pacman.dir == DIR_RIGHT) {
        pacman.dir = DIR_LEFT;
    } 
    else {
        if (!pm_maze_is_wall_pacman(pacman.x - 1, pacman.y)) {      // Check if Left pacman not wall
            pacman.dir = DIR_LEFT;
        } 
        else if (!pm_maze_is_wall_pacman(pacman.x + 1, pacman.y)) { // Check if Right pacman not wall
            pacman.dir = DIR_RIGHT;
        }
    }
}

void pm_pacman_toggle_vertical() {
    if (pacman.dir == DIR_UP) {
        pacman.dir = DIR_DOWN;
    } 
    else if (pacman.dir == DIR_DOWN) {
        pacman.dir = DIR_UP;
    } 
    else {
        if (!pm_maze_is_wall_pacman(pacman.x, pacman.y - 1)) {      // Check if Up pacman not wall
            pacman.dir = DIR_UP;
        } 
        else if (!pm_maze_is_wall_pacman(pacman.x, pacman.y + 1)) { // Check if Down pacman not wall
            pacman.dir = DIR_DOWN;
        }
    }
}

void pm_pacman_update_move() {
    if (pacman.dir == DIR_UP &&
        !pm_maze_is_wall_pacman(pacman.x, pacman.y - 1)) {
        pacman.y--;
    }
    else if (pacman.dir == DIR_DOWN &&
             !pm_maze_is_wall_pacman(pacman.x, pacman.y + 1)) {
        pacman.y++;
    }
    else if (pacman.dir == DIR_LEFT &&
             !pm_maze_is_wall_pacman(pacman.x - 1, pacman.y)) {
        pacman.x--;
    }
    else if (pacman.dir == DIR_RIGHT &&
             !pm_maze_is_wall_pacman(pacman.x + 1, pacman.y)) {
        pacman.x++;
    }
    else {
        pacman.dir = DIR_IDLE;
    }
}