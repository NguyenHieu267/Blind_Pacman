#include "pm_game_core.h"
#include "eeprom.h"
#include <stdlib.h>
#include <string.h>

#define EEPROM_SCORE_ADDR   0x0010
#define EEPROM_SETTING_ADDR 0x0100

uint8_t set_difficulty = 0; 
uint8_t set_sound_on   = 1; 
uint8_t set_time_limit = 40; 
uint32_t top_times[3] = {999, 999, 999}; 

Character pacman, blinky, pinky, inky, clyde;
uint8_t game_maze[32][16];
int frightened_timer = 0; 
int dots_left = 0;
int game_time_left = 40; 
int time_played = 0;
static uint16_t tick_counter = 0;

static const uint8_t game_maze_base[32][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, {1,4,0,0,0,0,0,1,1,0,0,0,0,0,4,1}, 
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, 
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, 
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, 
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, 
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, 
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, 
    {2,2,2,2,2,1,0,1,1,0,1,2,2,2,2,2}, {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, 
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, {1,0,1,1,0,1,1,3,3,1,1,0,1,1,0,1}, 
    {1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1}, {1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1}, 
    {1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1}, {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, 
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, 
    {1,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1}, {1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1}, 
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, 
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, 
    {1,4,1,1,1,1,0,1,1,0,1,1,1,1,4,1}, {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}  
};

static bool is_data_loaded = false;

void pm_load_system_data() {
    if (is_data_loaded) return;
    eeprom_read(EEPROM_SCORE_ADDR, (uint8_t*)top_times, sizeof(top_times));
    if (top_times[0] == 0 || top_times[0] > 9999) { top_times[0]=999; top_times[1]=999; top_times[2]=999; }
    uint8_t settings[3];
    eeprom_read(EEPROM_SETTING_ADDR, settings, 3);
    if (settings[0] <= 2) { 
        set_difficulty = settings[0]; set_sound_on = settings[1]; set_time_limit = settings[2];
        if (set_time_limit != 0 && (set_time_limit < 10 || set_time_limit > 150)) set_time_limit = 40;
    }
    is_data_loaded = true;
}

void pm_save_settings() {
    uint8_t settings[3] = {set_difficulty, set_sound_on, set_time_limit};
    eeprom_write(EEPROM_SETTING_ADDR, settings, 3);
}

void pm_update_leaderboard(uint32_t time) {
    for (int i = 0; i < 3; i++) {
        if (time < top_times[i]) {
            for (int j = 2; j > i; j--) top_times[j] = top_times[j-1];
            top_times[i] = time;
            eeprom_write(EEPROM_SCORE_ADDR, (uint8_t*)top_times, sizeof(top_times));
            break;
        }
    }
}

static bool is_wall_pacman(int x, int y) {
    if (x < 0 || x >= 16 || y < 0 || y >= 32) return true; 
    return (game_maze[y][x] == 1 || game_maze[y][x] == 3); 
}

static bool is_wall_ghost(int x, int y) {
    if (x < 0 || x >= 16 || y < 0 || y >= 32) return true; 
    return (game_maze[y][x] == 1);
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
        if (!is_wall_ghost(nx, ny)) {
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

static void reset_positions() {
    pacman = {8, 21, DIR_IDLE};
    blinky = {7, 18, DIR_UP}; pinky = {8, 18, DIR_UP};
    inky   = {7, 19, DIR_UP}; clyde = {8, 19, DIR_UP};
}

void pm_init_game() {
    memcpy(game_maze, game_maze_base, sizeof(game_maze_base));
    dots_left = 0;
    for(int r=0; r<32; r++) for(int c=0; c<16; c++) if(game_maze[r][c] == 0 || game_maze[r][c] == 4) dots_left++;
    frightened_timer = 0; time_played = 0; tick_counter = 0;
    game_time_left = set_time_limit;
    reset_positions();
}

void pm_change_dir(direction_t new_dir) {
    if (new_dir == DIR_UP || new_dir == DIR_DOWN) {
        if (pacman.dir == DIR_DOWN && new_dir == DIR_UP) pacman.dir = DIR_UP;
        else if (pacman.dir == DIR_UP && new_dir == DIR_DOWN) pacman.dir = DIR_DOWN;
        else {
            if (!is_wall_pacman(pacman.x, pacman.y - 1)) pacman.dir = DIR_UP;
            else if (!is_wall_pacman(pacman.x, pacman.y + 1)) pacman.dir = DIR_DOWN;
        }
    } else {
        if (pacman.dir == DIR_RIGHT && new_dir == DIR_LEFT) pacman.dir = DIR_LEFT;
        else if (pacman.dir == DIR_LEFT && new_dir == DIR_RIGHT) pacman.dir = DIR_RIGHT;
        else {
            if (!is_wall_pacman(pacman.x - 1, pacman.y)) pacman.dir = DIR_LEFT;
            else if (!is_wall_pacman(pacman.x + 1, pacman.y)) pacman.dir = DIR_RIGHT;
        }
    }
}

int pm_update_tick(uint16_t anim_tick) {
    tick_counter++;
    if (tick_counter % 8 == 0) {
        time_played++;
        if (set_time_limit > 0) {
            game_time_left--;
            if (game_time_left <= 0) return 2; // LOSE
        }
    }

    if (frightened_timer > 0) frightened_timer--;

    if (pacman.dir == DIR_UP && !is_wall_pacman(pacman.x, pacman.y - 1)) pacman.y--;
    else if (pacman.dir == DIR_DOWN && !is_wall_pacman(pacman.x, pacman.y + 1)) pacman.y++;
    else if (pacman.dir == DIR_LEFT && !is_wall_pacman(pacman.x - 1, pacman.y)) pacman.x--;
    else if (pacman.dir == DIR_RIGHT && !is_wall_pacman(pacman.x + 1, pacman.y)) pacman.x++;
    else pacman.dir = DIR_IDLE; 

    if (game_maze[pacman.y][pacman.x] == 0) { game_maze[pacman.y][pacman.x] = 2; dots_left--; } 
    else if (game_maze[pacman.y][pacman.x] == 4) { game_maze[pacman.y][pacman.x] = 2; dots_left--; frightened_timer = 60; }

    if (dots_left == 0) { pm_update_leaderboard(time_played); return 1; } // WIN

    if (anim_tick % 4 == 0) {
        bool is_fright = (frightened_timer > 0);
        blinky.dir = get_ghost_move(&blinky, pacman.x, pacman.y, is_fright);
        int px = pacman.x, py = pacman.y;
        if (pacman.dir==DIR_UP) py-=4; else if(pacman.dir==DIR_DOWN) py+=4; else if(pacman.dir==DIR_LEFT) px-=4; else if(pacman.dir==DIR_RIGHT) px+=4;
        pinky.dir = get_ghost_move(&pinky, px, py, is_fright);
        inky.dir = get_ghost_move(&inky, px + (px - blinky.x), py + (py - blinky.y), is_fright);
        clyde.dir = (dist_sq(clyde.x, clyde.y, pacman.x, pacman.y) > 64) ? get_ghost_move(&clyde, pacman.x, pacman.y, is_fright) : get_ghost_move(&clyde, 0, 31, is_fright);

        apply_move(&blinky); apply_move(&pinky); apply_move(&inky); apply_move(&clyde);
    }

    auto check = [](Character* g) {
        if(pacman.x == g->x && pacman.y == g->y) {
            if (frightened_timer > 0) { g->x = 7; g->y = 18; g->dir = DIR_UP; } 
            else { reset_positions(); } 
        }
    };
    check(&blinky); check(&pinky); check(&inky); check(&clyde);

    return 0; // Đang chơi
}