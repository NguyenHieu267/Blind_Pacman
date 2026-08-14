#include "pm_game_ghost.h"

void pm_ghost_init() {
    blinky = {7, 18, DIR_UP}; 
    pinky  = {8, 18, DIR_UP};
    inky   = {7, 19, DIR_UP}; 
    clyde  = {8, 19, DIR_UP};
}

static int dist_sq(int x1, int y1, int x2, int y2) { return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1); }

static direction_t get_ghost_move(Character* ghost, int target_x, int target_y, bool is_fright) {
    int min_dist = 99999; direction_t best_dir = DIR_IDLE; bool moved = false;
    if (ghost->y >= 18 && ghost->y <= 19 && ghost->x >= 6 && ghost->x <= 9) { target_x = 7; target_y = 16; is_fright = false; }
    
    struct Move { direction_t dir; int dx; int dy; direction_t opp; };
    Move moves[4] = { {DIR_UP,0,-1,DIR_DOWN}, {DIR_LEFT,-1,0,DIR_RIGHT}, {DIR_DOWN,0,1,DIR_UP}, {DIR_RIGHT,1,0,DIR_LEFT} };
    
    for (int i=0; i<4; i++) {
        if (ghost->dir != DIR_IDLE && moves[i].opp == ghost->dir) continue; 
        int nx = ghost->x + moves[i].dx, ny = ghost->y + moves[i].dy;
        if (!pm_maze_is_wall_ghost(nx, ny)) {
            int d = is_fright ? (rand() % 1000) : dist_sq(nx, ny, target_x, target_y);
            if (d < min_dist) { min_dist = d; best_dir = moves[i].dir; moved = true; }
        }
    }
    if (!moved) { 
        if (ghost->dir == DIR_UP) best_dir = DIR_DOWN; else if (ghost->dir == DIR_DOWN) best_dir = DIR_UP;
        else if (ghost->dir == DIR_LEFT) best_dir = DIR_RIGHT; else if (ghost->dir == DIR_RIGHT) best_dir = DIR_LEFT;
    }
    return best_dir;
}

static void apply_move(Character* c) {
    if (c->dir == DIR_UP) c->y--; else if (c->dir == DIR_DOWN) c->y++;
    else if (c->dir == DIR_LEFT) c->x--; else if (c->dir == DIR_RIGHT) c->x++;
}

void pm_ghosts_update_move(bool is_frightened) {
    blinky.dir = get_ghost_move(&blinky, pacman.x, pacman.y, is_frightened);
    int px = pacman.x, py = pacman.y;
    if (pacman.dir==DIR_UP) py-=4; else if(pacman.dir==DIR_DOWN) py+=4; else if(pacman.dir==DIR_LEFT) px-=4; else if(pacman.dir==DIR_RIGHT) px+=4;
    pinky.dir = get_ghost_move(&pinky, px, py, is_frightened);
    inky.dir = get_ghost_move(&inky, px + (px - blinky.x), py + (py - blinky.y), is_frightened);
    clyde.dir = (dist_sq(clyde.x, clyde.y, pacman.x, pacman.y) > 64) ? get_ghost_move(&clyde, pacman.x, pacman.y, is_frightened) : get_ghost_move(&clyde, 0, 31, is_frightened);

    apply_move(&blinky); apply_move(&pinky); apply_move(&inky); apply_move(&clyde);
}

void pm_ghost_check_collision(bool is_frightened) {
    auto check = [&](Character* g) {
        if(pacman.x == g->x && pacman.y == g->y) {
            if (is_frightened) { g->x = 7; g->y = 18; g->dir = DIR_UP; } 
            else { 
                pm_pacman_init(); 
                pm_ghost_init(); 
            } 
        }
    };
    check(&blinky); check(&pinky); check(&inky); check(&clyde);
}