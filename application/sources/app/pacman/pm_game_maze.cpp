#include "pm_game_maze.h"

#define MAZE_WIDTH  16
#define MAZE_HEIGHT 32

uint8_t game_maze[MAZE_HEIGHT][MAZE_WIDTH];
int dots_left = 0;

//==============MAZE RULE==============
/*
0: dot                  .
1: wall                 #
2: Way for ghost        =    
3: Ghost area           G
4: cherry               C
*/

static const uint8_t game_maze_base[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // ################
    {1,4,0,0,0,0,0,1,1,0,0,0,0,0,4,1}, // #C.....##.....C#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // #..............#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, // #......##......#
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, // ######.##.######
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, // ######.##.######
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // #..............#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, // #......##......#
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, // ######.##.######
    {2,2,2,2,2,1,0,1,1,0,1,2,2,2,2,2}, // =====#.##.#=====
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}, // ######.##.######
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // #..............#
    {1,0,1,1,0,1,1,3,3,1,1,0,1,1,0,1}, // #.##.##GG##.##.#
    {1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1}, // #.##.#====#.##.#
    {1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1}, // #.##.#====#.##.#
    {1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1}, // #.##.######.##.#
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // #..............#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1}, // #....#.##.#....#
    {1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1}, // ####.#.##.#.###
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // #..............#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1}, // #.####.##.####.#
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1}, // #......##......#
    {1,4,1,1,1,1,0,1,1,0,1,1,1,1,4,1}, // #C####.##.####C#
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}  // ################
};

void pm_maze_init() {
    dots_left = 0;
    for(int r=0; r<MAZE_HEIGHT; r++) {
        for(int c=0; c<MAZE_WIDTH; c++) {
            game_maze[r][c] = game_maze_base[r][c];
            if (game_maze[r][c] == MAZE_DOT || 
                game_maze[r][c] == MAZE_CHERRY) 
            
                dots_left++;
        }
    }
}

bool pm_maze_is_wall_pacman(int x, int y) {
    if (x < 0 ||
        x >= MAZE_WIDTH || 
        y < 0 || 
        y >= MAZE_HEIGHT) 
        
        return true; 

    return (game_maze[y][x] == MAZE_WALL || 
            game_maze[y][x] == MAZE_GHOST); 
}

bool pm_maze_is_wall_ghost(int x, int y) {
    if (x < 0 || 
        x >= MAZE_WIDTH || 
        y < 0 || 
        y >= MAZE_HEIGHT) 
        
        return true; 

    return (game_maze[y][x] == MAZE_WALL);
}

int pm_maze_eat_dot(int x, int y) {
    if (game_maze[y][x] == MAZE_DOT){
        game_maze[y][x] = MAZE_EMPTY; 
        dots_left--; 
        return 1; // Eat dot
    } 
    else if (game_maze[y][x] == MAZE_CHERRY) {
        game_maze[y][x] = MAZE_EMPTY; 
        dots_left--; 
        return 2; // Eat cherry
    }
    return 0; // Nothing
}


//==================================================
// RENDER MAZE
//==================================================

void pm_maze_render(void)
{
    const int CELL_W = 8;
    const int CELL_H = 2;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            int px = x * CELL_W;
            int py = y * CELL_H;
            switch (game_maze[y][x]) {
            case MAZE_WALL:
                for (int dy = 0; dy < CELL_H; dy++) {
                    for (int dx = 0; dx < CELL_W; dx++) {
                        view_render.drawPixel(
                            px + dx,
                            py + dy,
                            WHITE
                        );
                    }
                }
                break;

            case MAZE_DOT:
                view_render.drawPixel(
                    px + CELL_W / 2,
                    py,
                    WHITE
                );
                break;

            case MAZE_CHERRY:
                view_render.drawPixel(
                    px + CELL_W / 2,
                    py,
                    WHITE
                );
                break;

            default: break;
            }
        }
    }
}