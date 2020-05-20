#include <avr/io.h>
#include "lcd.h"

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>





#define BUFFSIZE 256



#define VIEW_WIDTH 25 // Minimum recommended is 20, max is 52
#define VIEW_HEIGHT 25 // Minimum recommended is 20, max is 28

#define MAP_WIDTH 52 // Minimum recommended is 20
#define MAP_HEIGHT 28 // Minimum recommended is 20
#define MAX_ROOM_WIDTH 5 // Minimum 5
#define MAX_ROOM_HEIGHT 5 // Minimum 5
#define ATTEMPTS_ROOM 1000

#define GAME_EMPTY 0
#define GAME_WALL 1
#define GAME_ROOM 2
#define GAME_PATH 3
#define GAME_DOOR 4





int generateRandomNum(int lowest, int highest);

int addRoomsToMap(int* map); // Returns number of doors added
void fillMapWithEmpty(int* map);

bool checkRoom(int* map, int sx, int sy, int ex, int ey);
void drawRoom(int* map, int sx, int sy, int ex, int ey);

void printView(int* map, int[] start);
void printMap(int* map);




// This program makes use of Klaus-Peter Zauner's lcd library which is included in the git repo
int main()
{
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
    init_lcd();
    srand(time(NULL));

    int* map = (int*)malloc(MAP_HEIGHT * MAP_WIDTH * sizeof(int));
    int* doors;
    int doorcount;

    //temporary
    int[] viewStartLocation = int[2] { 0, 0 };

    fillMapWithEmpty(map);
	doorcount = addRoomsToMap(map);

    printView(map, viewStartLocation);
    
    free(doors);
    free(map);
    return 0;
}

void printView(int* map, int[] start)
{ // Can fail to behave as expected if the map is larger than the screen
    for (int i = start[1]; i <= VIEW_HEIGHT; i++) {
        for (int j = start[0]; j <= VIEW_WIDTH; j++) {
            if (*(map + i * VIEW_WIDTH + j) == GAME_EMPTY) {
                display_string(" ");
            } else if (*(map + i * VIEW_WIDTH + j) == GAME_WALL) {
                display_string("W");
            } else if (*(map + i * VIEW_WIDTH + j) == GAME_ROOM) {
                display_string("-");
            } else if (*(map + i * VIEW_WIDTH + j) == GAME_PATH) {
                display_string("#");
            } else if (*(map + i * VIEW_WIDTH + j) == GAME_DOOR) {
                display_string("+");
            }
        }
        display_string("\n");
    }
}

void printMap(int* map)
{ // Can fail to behavee as expected if the map is larger than the screen
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
            } else if (*(map + i * MAP_WIDTH + j) == GAME_DOOR) {
                display_string("+");
            }
        }
        display_string("\n");
    }
}

int addRoomsToMap(int* map, int* newDoors)
{
    int roomAttempts = ATTEMPTS_ROOM;
    int doorCount = 0;
    
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

        int numofDoors = generateRandomNum(1, 3);
        int* oldDoors;

        while (numOfDoors > 0) {
            int wall = generateRandomNum(0, 3);
            int position;
            int[] newDoor;

            if (wall == 0) {
                position = generateRandomNum(roomStartX + 1, roomEndX - 1);
                newDoor = int[2] { position, roomStartY };
            } else if (wall == 1) {
                position = generateRandomNum(roomStarty + 1, roomEndy - 1);
                newDoor = int[2] { roomEndX, position };
            } else if (wall == 2) {
                position = generateRandomNum(roomStartX + 1, roomEndX - 1);
                newDoor = int[2] { position, roomEndY };
            } else if (wall == 3) {
                position = generateRandomNum(roomStarty + 1, roomEndy - 1);
                newDoor = int[2] { roomStartY, position };
            }
            doorCount += 1;
            numOfDoors -= 1;
            free(oldDoors);
            *(map + newDoor[1] * MAP_WIDTH + newDoor[0]) = GAME_DOOR;

            oldDoors = newDoors;
            newDoors = malloc(sizeof(int) * 2 * doorCount);
            for (int i = 0; i < doorCount - 1; i++) {
                *(newDoors + i * sizeof(int) * 2) = *(oldDoors + i * sizeof(int) * 2);
                *(newDoors + i * sizeof(int) * 2 + sizeof(int)) = *(oldDoors + i * sizeof(int) * 2 + sizeof(int));
            }
            *(newDoors + (doorCount - 1) * sizeof(int) * 2) = newDoor[0];
            *(newDoors + (doorCount - 1) * sizeof(int) * 2 + sizeof(int)) = newDoor[1];
        }
        free(oldDoors);

        return doorCount;
    }

    if (checkRoom(map, 40, 40, 50, 46)) {
        drawRoom(map, 45, 41, 50, 46);
    }
    return doorCount;
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

void fillMapWithEmpty(int* map)
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
