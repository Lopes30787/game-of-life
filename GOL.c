#include <stdio.h>

#include "grower.h"
#include "glider.h"
#include "beehive.h"

#define NUM_ITERATIONS 5000
#define SIZE 3000

int GOL[SIZE][SIZE] = {{0}};

int GOL_copy[SIZE][SIZE];

//Initialize the current pattern.
//TODO: Fazer esta merda automatica para nao ter de alterar os nomes das variaveis quando quero testar um pattern diferente.
void initialize() {
    for(int i = 0; i < GROWER_HEIGHT; i++) {
        for(int j = 0; j < GROWER_WIDTH; j++) {
            GOL[i+1500][j+1500] = grower[i][j];
        }
    }
}

//Create copy of gol for neighbour counting purposes.
void copy_gol() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            GOL_copy[i][j] = GOL[i][j];
        }
    }
}

//TODO: Paralelizar isto
int iteration() {
    copy_gol();
    int alive;
    int population = 0;

    for(int i = 1; i < SIZE-1; i++) {
        for(int j = 1; j < SIZE-1; j++) {
            alive = 0;

            //Check number of live neighbours.
            if (GOL_copy[i+1][j] == 1) {
                alive++;
            }
            if (GOL_copy[i+1][j+1] == 1) {
                alive++;
            }
            if (GOL_copy[i+1][j-1] == 1) {
                alive++;
            }
            if (GOL_copy[i][j+1] == 1) {
                alive++;
            }
            if (GOL_copy[i][j-1] == 1) {
                alive++;
            }
            if (GOL_copy[i-1][j+1] == 1) {
                alive++;
            }
            if (GOL_copy[i-1][j] == 1) {
                alive++;
            }
            if (GOL_copy[i-1][j-1] == 1) {
                alive++;
            }

            //Any live cell with fewer than two or more than three live neighbours dies.
            if ((alive < 2 || alive > 3) && GOL_copy[i][j] == 1) {
                GOL[i][j] = 0;
            }

            //Any live cell with two or three live neighbours lives, unchanged, to the next generation.
            else if ((alive == 2 || alive == 3) && GOL_copy[i][j] == 1) {
                GOL[i][j] = 1;
                population++;
            }

            //Any dead cell with exactly three live neighbours will come to life.
            else if (alive == 3 && GOL_copy[i][j] == 0) {
                GOL[i][j] = 1;
                population++;
            }
        }
    }
    return population;
}

int main() {
    initialize();

    for (int i = 0; i<100; i++) {
        int population = iteration();

        //printf("%d\n", population);
        if (i == 9 || i == 99) {
            printf("%d\n", population);
        }
    }

    return 0;
}