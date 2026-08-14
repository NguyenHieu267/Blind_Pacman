#ifndef __PM_GAME_CORE_H__
#define __PM_GAME_CORE_H__

#include "pm_game.h"

extern Character pacman, blinky, pinky, inky, clyde;
extern int frightened_timer;
extern int dots_left;
extern int game_time_left;
extern int time_played;
extern uint16_t anim_tick;

extern void pm_game_core_handle(ak_msg_t* msg);
extern void pm_ghost_init();
extern void pm_ghosts_update_move(bool is_frightened);
extern void pm_ghost_check_collision(bool is_frightened);

#endif // __PM_GAME_CORE_H__