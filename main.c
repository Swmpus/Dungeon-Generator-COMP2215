/*
    WHOLE PROGRAM VERY SLOW CAUSE LOTS OF MEMORY BEING COPIED
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAP_WIDTH 101 // Minimum recommended is 20
#define MAP_HEIGHT 101 // Minimum recommended is 20
#define MAX_ROOM_WIDTH 10 // Minimum 5
#define MAX_ROOM_HEIGHT 10 // Minimum 5
#define ATTEMPTS_ROOM 50

#define GAME_EMPTY 1
#define GAME_WALL 2
#define GAME_ROOM 3
#define GAME_PATH 4





int generateRandomNum(int lowest, int highest);

bool checkRoom(int* map, int sx, int sy, int ex, int ey);

void generateMap(int* map);

void drawRoom(int* map, int sx, int sy, int ex, int ey);

void printMap(int* map);





int main()
{
    int* map = (int*)malloc(MAP_HEIGHT * MAP_WIDTH * sizeof(int));

    srand(time(NULL));
    generateMap(map);

    printf("Map Generated\n");

    int roomAttempts = ATTEMPTS_ROOM;

    while (roomAttempts > 0) {
        int roomStartX = generateRandomNum(3, MAP_WIDTH - 3 - MAX_ROOM_WIDTH);
        int roomEndX = roomStartX;
        int roomStartY = generateRandomNum(3, MAP_HEIGHT - 3 - MAX_ROOM_HEIGHT);
        int roomEndY = roomStartY;

        do {
            roomEndX = abs(generateRandomNum(roomStartX + 3, roomStartX + MAX_ROOM_WIDTH));
        } while (roomEndX >= MAP_WIDTH - 3);
        do {
            roomEndY = abs(generateRandomNum(roomStartY + 3, roomStartY + MAX_ROOM_HEIGHT));
        } while (roomEndY >= MAP_HEIGHT - 3);

        printf("Start: %d, %d; End %d, %d\n", roomStartX, roomEndX, roomStartY, roomEndY);
        if (checkRoom(map, roomStartX, roomStartY, roomEndX, roomEndY)) {
            drawRoom(map, roomStartX, roomStartY, roomEndX, roomEndY);
        }
        roomAttempts--;
    }
    printMap(map);
    printf("Map Printed\n");
    
    free(map);
    return 0;
}

void printMap(int* map)
{
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (*(map + i * MAP_WIDTH + j) == GAME_EMPTY) {
                printf(" ");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_WALL) {
                printf("W");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_ROOM) {
                printf("-");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_PATH) {
                printf("#");
            }
        }
        printf("\n");
    }
}


void drawRoom(int* map, int sx, int sy, int ex, int ey)
{            
    for (int j = sy; j <= ey; j++ ) {
        for (int i = sx; i <= ex; i++ ) {
            *(map + i * MAP_WIDTH + j) = GAME_WALL;
        }
    }
    for (int j = sy + 1; j <= ey - 1; j++) {
        for (int i = sx + 1; i <= ex - 1; i++) {
            *(map + i * MAP_WIDTH + j) = GAME_ROOM;
        }
    }
}

bool checkRoom(int* map , int sx, int sy, int ex, int ey)
{
    for (int j = sy - 1; j <= ey + 1; j++ ) {
        for (int i = sx - 1; i <= ex + 1; i++ ) {
            if (*(map + i * MAP_WIDTH + j) != GAME_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

void generateMap(int* map)
{
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            *(map + i * MAP_WIDTH + j) = GAME_EMPTY;
        }
    }
}

int generateRandomNum(int lowest, int highest)
{
    return (rand() % (highest - lowest + 1)) + lowest;
}