#include <avr/io.h>
#include "lcd.h"

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>





#define BUFFSIZE 256





#define MAP_WIDTH 52 // Minimum recommended is 20
#define MAP_HEIGHT 28 // Minimum recommended is 20
#define MAX_ROOM_WIDTH 5 // Minimum 5
#define MAX_ROOM_HEIGHT 5 // Minimum 5
#define ATTEMPTS_ROOM 1000

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
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
    init_lcd();

    int* map = (int*)malloc(MAP_HEIGHT * MAP_WIDTH * sizeof(int));

    srand(time(NULL));
    generateMap(map);

    int roomAttempts = ATTEMPTS_ROOM;
    
    while (roomAttempts > 0) {
        int roomStartX = generateRandomNum(3, MAP_WIDTH - 7);
        int roomEndX = roomStartX;
        int roomStartY = generateRandomNum(3, MAP_HEIGHT - 7);
        int roomEndY = roomStartY;

        do {
            roomEndX = abs(generateRandomNum(roomStartX + 3, roomStartX + MAX_ROOM_WIDTH));
        } while (roomEndX > MAP_WIDTH - 3);
        do {
            roomEndY = abs(generateRandomNum(roomStartY + 3, roomStartY + MAX_ROOM_HEIGHT));
        } while (roomEndY > MAP_HEIGHT - 3);

        if (checkRoom(map, roomStartX, roomStartY, roomEndX, roomEndY)) {
            drawRoom(map, roomStartX, roomStartY, roomEndX, roomEndY);
        }
        roomAttempts--; 
    }

    if (checkRoom(map, 40, 40, 50, 46)) {
        drawRoom(map, 45, 41, 50, 46);
    }
    printMap(map);
    free(map);
    return 0;
}

void printMap(int* map)
{
    for (int i = 0; i <= MAP_HEIGHT; i++) {
        for (int j = 0; j <= MAP_WIDTH; j++) {
            if (*(map + i * MAP_WIDTH + j) == GAME_EMPTY) {
                display_string(" ");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_WALL) {
                display_string("W");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_ROOM) {
                display_string("-");
            } else if (*(map + i * MAP_WIDTH + j) == GAME_PATH) {
                display_string("#");
            }
        }
        display_string("\n");
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
    for (int i = 0; i <= MAP_HEIGHT; i++) {
        for (int j = 0; j <= MAP_WIDTH; j++) {
            *(map + i * MAP_WIDTH + j) = GAME_EMPTY;
        }
    }
}

int generateRandomNum(int lowest, int highest)
{
    return (rand() % (highest - lowest + 1)) + lowest;
}
