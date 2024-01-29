#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grower.h"
#include "glider.h"
#include "beehive.h"

#define NUM_ITERATIONS 5000
#define SIZE 3000

int GOL[SIZE][SIZE] = {{0}};

int GOL_copy[SIZE][SIZE];

//Initialize the current pattern.
void initialize(int pattern) {

    //Initialize grower pattern
    if (pattern == 1) {
        for(int i = 0; i < GROWER_HEIGHT; i++) {
            for(int j = 0; j < GROWER_WIDTH; j++) {
                GOL[i+1500][j+1500] = grower[i][j];
            }
        }
    }

    //Initialize glider pattern
    if (pattern == 2) {
        for(int i = 0; i < GLIDER_HEIGHT; i++) {
            for(int j = 0; j < GLIDER_WIDTH; j++) {
                GOL[i+1500][j+1500] = glider[i][j];
            }
        }
    }

    //Initialize beehive pattern
    if (pattern == 3) {
        for(int i = 0; i < BEEHIVE_HEIGHT; i++) {
            for(int j = 0; j < BEEHIVE_WIDTH; j++) {
                GOL[i+1500][j+1500] = beehive[i][j];
            }
        }
    }
}

//Create copy of gol for neighbour counting purposes.
void copy_gol() {
    #pragma omp parallel for
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            GOL_copy[i][j] = GOL[i][j];
        }
    }
}

void iteration() {
    copy_gol();
    int alive;
    int population = 0;

    //Parallelization of the main cycle
    #pragma omp parallel for private(alive)
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
            }

            //Any dead cell with exactly three live neighbours will come to life.
            else if (alive == 3 && GOL_copy[i][j] == 0) {
                GOL[i][j] = 1;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int pattern;
    if (argc == 2) {
        if (strcmp(argv[1], "grower") == 0) {
            pattern = 1;
        } 
        else if (strcmp(argv[1], "glider") == 0) {
            pattern = 2;
        } 
        else if (strcmp(argv[1], "beehive") == 0) {
            pattern = 3;
        } 
        else {
            printf("Unknown argument.\n");
            return 0;
        }
    }

    else {
        printf("Wrong number of arguments.\n");
        return 0;
    }

    // Get timing
    double start,end;
    start=omp_get_wtime();

    initialize(pattern);

    for (int i = 0; i<NUM_ITERATIONS; i++) {
        iteration();
    }

    // Stop timing
    end=omp_get_wtime();

    // Print result
    printf("Obtained in %f seconds\n", end-start);

    //Check Final Population
    int population = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if (GOL[i][j] == 1) {
                population++;
            }
        }
    }
    printf("Final Population: %d\n", population);

    return 0;
}