#include "pm_game_ghost.h"

#define GHOST_HOME_X1      7
#define GHOST_HOME_X2      8 
#define GHOST_EXIT_Y       16

#define GHOST_HOME_MIN_X   6
#define GHOST_HOME_MAX_X   9
#define GHOST_HOME_MIN_Y   18
#define GHOST_HOME_MAX_Y   19

#define PINKY_LOOK_AHEAD   4
#define CLYDE_DISTANCE     64

// Init 4 pacman at ghost house (central rectangle in maze)
void pm_ghost_init() {    
    blinky = {GHOST_HOME_X1, GHOST_HOME_MIN_Y, DIR_UP}; 
    pinky  = {GHOST_HOME_X2, GHOST_HOME_MIN_Y, DIR_UP}; 
    inky   = {GHOST_HOME_X1, GHOST_HOME_MAX_Y, DIR_UP}; 
    clyde  = {GHOST_HOME_X2, GHOST_HOME_MAX_Y, DIR_UP};
}

// Calculate the distance between ghost and pacman
static int dist_sq(int x1, int y1, int x2, int y2) { 
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1); 
}

// Calculate the best_dir to find pacman 
static direction_t get_ghost_move(Character* ghost, int target_x, int target_y, bool is_fright) {
    if (ghost->y >= GHOST_HOME_MIN_Y &&         // If ghosts are in ghost house -> Target: go out of ghost house
        ghost->y <= GHOST_HOME_MAX_Y && 
        ghost->x >= GHOST_HOME_MIN_X && 
        ghost->x <= GHOST_HOME_MAX_X) 
    { 
        target_x = GHOST_HOME_X1; 
        target_y = GHOST_EXIT_Y; 
        is_fright = false;
    }

    Move moves[4] = {                   
        {DIR_UP,    0, -1, DIR_DOWN}, 
        {DIR_LEFT, -1,  0, DIR_RIGHT}, 
        {DIR_DOWN,  0,  1, DIR_UP}, 
        {DIR_RIGHT, 1,  0, DIR_LEFT} 
    };
    
    int min_dist = 999; 
    direction_t best_dir = DIR_IDLE; 
    bool moved = false;

    for (int i = 0; i < 4; i++) {
        if (ghost->dir != DIR_IDLE && moves[i].opp == ghost->dir) { // Check if ghost dir now != ghost dir future
            continue; 
        }
        
        int next_x = ghost->x + moves[i].dx;
        int next_y = ghost->y + moves[i].dy;

        if (!pm_maze_is_wall_ghost(next_x, next_y)) {
            int d = is_fright ? (rand() % 1000) : dist_sq(next_x, next_y, target_x, target_y);
            if (d < min_dist) { 
                min_dist = d; 
                best_dir = moves[i].dir; 
                moved = true; 
            }
        }
    }

    if (!moved) { 
        switch (ghost->dir) {
            case DIR_UP:    
                best_dir = DIR_DOWN;  
                break;

            case DIR_DOWN:  
                best_dir = DIR_UP;    
                break;

            case DIR_LEFT:  
                best_dir = DIR_RIGHT; 
                break;

            case DIR_RIGHT: 
                best_dir = DIR_LEFT;  
                break;

            default:        
                best_dir = DIR_IDLE;  
                break;
        }
    }

    return best_dir;
}

static void apply_move(Character* c) {
    switch (c->dir) {
        case DIR_UP:    
            c->y--; 
            break;

        case DIR_DOWN:  
            c->y++; 
            break;

        case DIR_LEFT:  
            c->x--; 
            break;

        case DIR_RIGHT: 
            c->x++; 
            break;

        default: break;
    }
}

void pm_ghosts_update_move(bool is_frightened) {
    // Blinky (RED): Chase Pac-Man's position.
    blinky.dir = get_ghost_move(&blinky, pacman.x, pacman.y, is_frightened);
    
    // Pinky (Pink): Follow 4 tiles ahead of Pac-Man
    int px = pacman.x;
    int py = pacman.y;
    switch (pacman.dir) {
        case DIR_UP:    
            py -= PINKY_LOOK_AHEAD; 
            break;

        case DIR_DOWN:  
            py += PINKY_LOOK_AHEAD; 
            break;

        case DIR_LEFT:  
            px -= PINKY_LOOK_AHEAD; 
            break;

        case DIR_RIGHT: 
            px += PINKY_LOOK_AHEAD; 
            break;

        default: break;
    }
    pinky.dir = get_ghost_move(&pinky, px, py, is_frightened);
    
    // Inky (Blue): Surrounds based on the positions of Blinky and Pac-Man.
    int inky_target_x = px + (px - blinky.x);
    int inky_target_y = py + (py - blinky.y);
    inky.dir = get_ghost_move(&inky, inky_target_x, inky_target_y, is_frightened);
    
    // Clyde (Orange): Target Pac-Man if far away; Go to corner of the map if close (< 8 tiles).
    if (dist_sq(clyde.x, clyde.y, pacman.x, pacman.y) > CLYDE_DISTANCE) {
        clyde.dir = get_ghost_move(&clyde, pacman.x, pacman.y, is_frightened);
    } else {
        clyde.dir = get_ghost_move(&clyde, 0, 31, is_frightened);
    }

    apply_move(&blinky); 
    apply_move(&pinky); 
    apply_move(&inky); 
    apply_move(&clyde);
}

static void check_ghost_collision(Character* ghost, bool is_frightened)
{
    if (pacman.x != ghost->x || pacman.y != ghost->y) return;

    if (is_frightened) {
        ghost->x = GHOST_HOME_X1;
        ghost->y = GHOST_HOME_MIN_Y;
        ghost->dir = DIR_UP;
    } else {
        pm_pacman_init();
        pm_ghost_init();
    }
}

void pm_ghost_check_collision(bool is_frightened)
{
    check_ghost_collision(&blinky, is_frightened);
    check_ghost_collision(&pinky, is_frightened);
    check_ghost_collision(&inky, is_frightened);
    check_ghost_collision(&clyde, is_frightened);
}