// --- pm_game_ghost.h ---
#ifndef __PM_GAME_GHOST_H__
#define __PM_GAME_GHOST_H__

#include "pm_game.h"

extern void pm_ghost_init();
extern void pm_ghosts_update_move(bool is_frightened);
extern void pm_ghost_check_collision(bool is_frightened);

#endif