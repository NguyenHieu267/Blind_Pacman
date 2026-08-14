// --- pm_game_pacman.h ---
#ifndef __PM_GAME_PACMAN_H__
#define __PM_GAME_PACMAN_H__

#include "pm_game.h"

extern void pm_pacman_init();
extern void pm_pacman_change_dir(direction_t new_dir);
extern void pm_pacman_update_move();

#endif