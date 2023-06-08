#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 128
#define HEIGHT 128
#define ALIVE_VIEW '#'
#define DEAD_VIEW ' '


typedef struct {
    bool next_state; // 
    bool current_state;    // dead or alive
    char view;    // . or empty space
} Cell;

time_t t;
Cell grid[WIDTH][HEIGHT];


void show_grid(int epochs){
    for (int y = 0; y < HEIGHT; y++){
        //printf("Y: %d\t",y);
        for (int x = 0; x < WIDTH; x++){
            fputc(grid[y][x].view,stdout);
        }
        fputc('\n',stdout);
    }
    printf("Epochs: %d\n",epochs);
}

void clear_grid(char view){
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            Cell cell = {.next_state = false, .current_state = false, .view = view};
            grid[y][x] = cell;
        }
    }
}

void clear_window(){
    printf("\e[1;1H\e[2J");
}

void setup_grid(char view) {
    Cell alive_cell = {.next_state = false, .current_state = true, .view = view };
    for (int i = 0; i < HEIGHT * WIDTH; i++){
        int y = (rand() % (HEIGHT));
        int x = (rand() % (WIDTH));
        grid[y][x] = alive_cell;
    }
}


int check_neighbors(int y,int x){
    int alive_neighbors = 0;
    // check top left node
    if (grid[y - 1][x - 1].current_state){
        alive_neighbors++;
    }
    // check directly above
    if (grid[y - 1][x].current_state){
        alive_neighbors++;
    }
    // check top right 
    if (grid[y - 1][x + 1].current_state){
        alive_neighbors++;
    }
    // check middle left 
    if (grid[y][x - 1].current_state){
        alive_neighbors++;
    }
    // check middle right
    if (grid[y][x + 1].current_state){
        alive_neighbors++;
    }
    // check bottom left
    if (grid[y + 1][x - 1].current_state){
        alive_neighbors++;
    }
    // check bottom middle
    if (grid[y + 1][x].current_state){
        alive_neighbors++;
    }
    // check bottom right
    if (grid[y + 1][x + 1].current_state){
        alive_neighbors++;
    }
    return alive_neighbors;
}

void update_grid(char view){
    Cell alive_next_tick = {.next_state = true, .current_state = false, .view = view};
    Cell dead = {.next_state = false, .current_state = false, .view = DEAD_VIEW};
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            int live = check_neighbors(y,x);
            //printf("Y:%d  X:%d -> %d\n",y,x,live);
            if (grid[y][x].current_state == true && (live == 2 || live == 3 )){
                //printf("x:%d y:%d survives\n",x,y);
                continue;
            }
            if (grid[y][x].current_state == false && live == 3){
                //printf("x:%d y:%d alive now\n",x,y);
                grid[y][x] = alive_next_tick;
                continue;
            }
            grid[y][x] = dead;
        }
    }
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            if (grid[y][x].next_state){
                grid[y][x].current_state = true;
                grid[y][x].next_state = false;
            }
        }
    }

}

int main(int argc, char** argv){
    srand((unsigned) time(&t));
    clear_grid(DEAD_VIEW);
    setup_grid(ALIVE_VIEW);
    int epochs = 0;
    while(true){
        show_grid(epochs++);
        update_grid(ALIVE_VIEW);
        getchar();
        clear_window();

    }
    return 0;
}
