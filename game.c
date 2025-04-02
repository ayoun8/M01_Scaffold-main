#include "gba.h"
#include "mode0.h"
#include "sprites.h"
#include "print.h"
#include "game.h"
#include <stdlib.h>

// .h files from tiled/usenti
#include "spritesheet.h"
#include "collisionMap.h"
#include "background1.h"
#include "tileset.h"
#include "collisionMap2.h"
#include "background2.h"
#include "tileset2.h"

// Global variable definitions
SPRITE player;
SPRITE fireballs [MAXFIREBALLS];
SPRITE rareCandy [MAXRARECANDY];
SPRITE healItem;
SPRITE lifeCount;

OBJ_ATTR shadowOAM[128];

// Direction enum
typedef enum {
    DOWN,
    UP,
    LEFT,
    RIGHT
} DIRECTION;

// Surrogate variables
int hOff;
int vOff;

// Variables
int lives;
int isPokemon = 0; // Trainer = 0, pokemon = 1
int evolution = 0; // 0 = Charmander, 1 = Charmeleon, 2 = Charizard
int rareCandiesCollected = 0;
int fireballsRemaining = 5;
int fireballDelayTimer = 0;

int gameLost = 0;
int level = 1;
int level2 = 0;


inline unsigned char colorAt(int x, int y){
    if (level == 1) {
        return ((unsigned char *) collisionMapBitmap) [OFFSET(x, y, MAPWIDTH)];
    } else if (level == 2) {
        return ((unsigned char *) collisionMap2Bitmap) [OFFSET(x, y, MAPWIDTH)];
    } else {
        return 0;
    }
    
}

void initGame() {
    initPlayer();
    initFireballs();
    initRareCandy();
    initHeal();

    lives = 3;
    fireballsRemaining = 5;
    rareCandiesCollected = 0;
    evolution = 0;
    isPokemon = 0;
    gameLost = 0;
    level = 1;

    lifeCount.x = 45;
    lifeCount.y = 65;
    lifeCount.width = 16;
    lifeCount.height = 16;
    lifeCount.oamIndex = 18;
}

void updateGame() {
    if (!gameLost) {
        updatePlayer();
        updateFireballs();
        updateRareCandy();
        updateHeal();

        if (level == 1 && rareCandiesCollected == 3 && exit1()) {
            evolution = 1;
            fireballsRemaining = 5;
            goToGame2();
        }

        // Delay lose screen after last fireball
        if (fireballsRemaining == 0 && fireballDelayTimer == 0) {
            fireballDelayTimer = 60;
        }

        if (fireballDelayTimer > 0) {
            fireballDelayTimer--;
            if (fireballDelayTimer == 0 && rareCandiesCollected < 3) {
                gameLost = 1;
            }
        }

        if (level == 2) {
            if (rareCandiesCollected == 3 && exit2()) {
                evolution = 2;
                goToWin();
            }
        }
    }
}

void drawGame() {
    drawPlayer();
    drawFireballs();
    drawRareCandy();
    // drawHeal();
    // drawHearts();
}

void initPlayer() {
    player.x = 51;
    player.y = 92;
    player.xVel = 1;
    player.yVel = 1;
    player.width = 16;
    player.height = 16;
    player.oamIndex = 0;
    player.timeUntilNextFrame = 5;
    player.direction = DOWN;
    player.currentFrame = 0;
    player.numFrames = 3;
    player.isAnimating = 0;
}

void updatePlayer() {
    player.isAnimating = 0;

    int leftX = player.x;
    int rightX = player.x + player.width - 1;
    int topY = player.y;
    int bottomY = player.y + player.height - 1;

    if (BUTTON_HELD(BUTTON_UP)) {
        player.direction = UP;
        if (player.y > 0 && colorAt(leftX, topY - player.yVel) != 0 && colorAt(rightX, topY - player.yVel) != 0) {
            player.y -= player.yVel;
        }
        player.isAnimating = 1;
    }

    if (BUTTON_HELD(BUTTON_DOWN)) {
        player.direction = DOWN;
        if (player.y + player.height < MAPHEIGHT && colorAt(leftX, bottomY + player.yVel) != 0 && colorAt(rightX, bottomY + player.yVel) != 0) {
            player.y += player.yVel;
        }
        player.isAnimating = 1;
    }

    if (BUTTON_HELD(BUTTON_LEFT)) {
        player.direction = LEFT;
        if (player.x > 0 && colorAt(leftX - player.xVel, topY) != 0 && colorAt(leftX - player.xVel, bottomY) != 0) {
            player.x -= player.xVel;
        }
        player.isAnimating = 1;
    }

    if (BUTTON_HELD(BUTTON_RIGHT)) {
        player.direction = RIGHT;
        if (player.x + player.width < MAPWIDTH && colorAt(rightX + player.xVel, topY) != 0 && colorAt(rightX + player.xVel, bottomY) != 0) {
            player.x += player.xVel;
        }
        player.isAnimating = 1;
    }

    if (BUTTON_PRESSED(BUTTON_B)) {
        isPokemon = !isPokemon;
    }

    if (isPokemon && BUTTON_PRESSED(BUTTON_A)) {
        placeFireball();
    }

    if (player.isAnimating) {
        player.timeUntilNextFrame--;
        if (player.timeUntilNextFrame <= 0) {
            player.timeUntilNextFrame = 10;
            player.currentFrame = (player.currentFrame + 1) % player.numFrames;
        }
    } else {
        player.currentFrame = 0;
        player.timeUntilNextFrame = 10;
    }

    hOff = player.x - SCREENWIDTH / 2;
    vOff = player.y - SCREENHEIGHT / 2;

    if (hOff < 0) {
        hOff = 0;
    }
    if (hOff > MAPWIDTH - SCREENWIDTH) {
        hOff = MAPWIDTH - SCREENWIDTH;
    }
    if (vOff < 0) {
        vOff = 0;
    }
    if (vOff > MAPHEIGHT - SCREENHEIGHT) {
        vOff = MAPHEIGHT - SCREENHEIGHT;
    }
}

void drawPlayer() {
    if (!isPokemon) {
        shadowOAM[0].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[0].attr1 = ATTR1_X(player.x - hOff) | ATTR1_SMALL;
        shadowOAM[0].attr2 = ATTR2_TILEID(player.direction * 2, player.currentFrame * 2);
    }
    
    else if (isPokemon) {
    shadowOAM[0].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
    shadowOAM[0].attr1 = ATTR1_X(player.x - hOff) | ATTR1_SMALL;
    shadowOAM[0].attr2 = ATTR2_TILEID(player.direction * 2 + 8, player.currentFrame * 2);
    }

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);
}

void initFireballs() {
    for (int i = 0; i < MAXFIREBALLS; i++) {
        fireballs[i].width = 16;
        fireballs[i].height = 16;
        fireballs[i].active = 0;
        fireballs[i].oamIndex = 10 + i;
    }
}

void placeFireball() {
    if (fireballsRemaining > 0) {
        for (int i = 0; i < MAXFIREBALLS; i++) {
            if (!fireballs[i].active) {
                fireballs[i].x = player.x + player.width / 2 - fireballs[i].width / 2;
                fireballs[i].y = player.y + player.height / 2 - fireballs[i].height / 2;
                fireballs[i].active = 1;
                fireballs[i].timer = 30;
                fireballsRemaining--;
                break;
            }
        }
    }
}

void updateFireballs() {
    for (int i = 0; i < MAXFIREBALLS; i++) {
        if (fireballs[i].active) {
            fireballs[i].timer--;
            if (fireballs[i].timer <= 0) {
                fireballs[i].active = 0;
                shadowOAM[fireballs[i].oamIndex].attr0 = ATTR0_HIDE;
                burn(fireballs[i].x, fireballs[i].y);
            }
        }
    }
}

void drawFireballs() {
    for (int i = 0; i < MAXFIREBALLS; i++) {
        if (fireballs[i].active) {
            shadowOAM[fireballs[i].oamIndex].attr0 = ATTR0_Y(fireballs[i].y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
            shadowOAM[fireballs[i].oamIndex].attr1 = ATTR1_X(fireballs[i].x - hOff) | ATTR1_SMALL;
            shadowOAM[fireballs[i].oamIndex].attr2 = ATTR2_TILEID(0, 6);
        }
    }
}

void burn(int x, int y) {
    // Flowers
    int tx = x / 8;
    int ty = y / 8;

    int offsets[4][2] = {
        {0, 0}, // top-left
        {-1, 0}, // top-right
        {0, -1}, // bottom-left
        {-1, -1} // bottom-right
    };

    for (int i = 0; i < 4; i++) {
        int topLeftX = tx + offsets[i][0];
        int topLeftY = ty + offsets[i][1];

        if (topLeftX < 0 || topLeftX >= 32 || topLeftY < 0 || topLeftY >= 32) {
            continue;
        }

        int topLeft = SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY, 32)];
        int topRight = SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY, 32)];
        int bottomLeft = SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY + 1, 32)];
        int bottomRight = SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY + 1, 32)];

        int match = 0;

        if (level == 1) {
            match = (topLeft == 0 && topRight == 1 && bottomLeft == 4 && bottomRight == 5);
        } else if (level == 2) {
            match = (topLeft == 22 && topRight == 23 && bottomLeft == 28 && bottomRight == 29);
        }

        if (match) {
            for (int dx = 0; dx < 2; dx++) {
                for (int dy = 0; dy < 2; dy++) {
                    SCREENBLOCK[28].tilemap[OFFSET(topLeftX + dx, topLeftY + dy, 32)] = 2;
                }
            }
            // SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY, 32)] = 2;
            // SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY, 32)] = 2;
            // SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY + 1, 32)] = 2;
            // SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY + 1, 32)] = 2;

            for (int j = 0; j < MAXRARECANDY; j++) {
                // Tiles
                int candyX = rareCandy[j].x / 8;
                int candyY = rareCandy[j].y / 8;
                if (!rareCandy[j].active &&
                    candyX == topLeftX &&
                    candyY == topLeftY) {
                    rareCandy[j].active = 1;
                }
            }
            break; // only burn one
        }
    }
}

void initRareCandy() {
    if (level == 1) {
        int candyX[] = {1 * 8, 3 * 8, 30 * 8};
        int candyY[] = {16 * 8, 30 * 8, 27 * 8};
    
        for (int i = 0; i < 3; i++) {
            rareCandy[i].x = candyX[i];
            rareCandy[i].y = candyY[i];
            rareCandy[i].width = 16;
            rareCandy[i].height = 16;
            // Hide
            rareCandy[i].active = 1;
            rareCandy[i].oamIndex = 50 + i;
        }
        for (int i = 3; i < MAXRARECANDY; i++) {
            rareCandy[i].active = 0;
        }
    } else if (level == 2) {
        int candyX[] = {30 * 8, 2 * 8, 28 * 8};
        int candyY[] = {3 * 8, 30 * 8, 28 * 8};
    
        for (int i = 0; i < 3; i++) {
            rareCandy[i].x = candyX[i];
            rareCandy[i].y = candyY[i];
            rareCandy[i].width = 16;
            rareCandy[i].height = 16;
            // Hide
            rareCandy[i].active = 1;
            rareCandy[i].oamIndex = 30 + i;
        }
    }
}

// void initRareCandy() {
//     int centerY = MAPHEIGHT / 2;
//     int startX = (MAPWIDTH / 2) - 24;

//     for (int i = 0; i < MAXRARECANDY; i++) {
//         rareCandy[i].x = startX + (i * 24);
//         rareCandy[i].y = centerY;
//         rareCandy[i].width = 16;
//         rareCandy[i].height = 16;
//         rareCandy[i].active = 1;
//         rareCandy[i].oamIndex = 30 + i;
//     }
// }

void updateRareCandy() {
    for (int i = 0; i < MAXRARECANDY; i++) {
        if (rareCandy[i].active && collision(player.x, player.y, player.width, player.height,
            rareCandy[i].x, rareCandy[i].y, rareCandy[i].width, rareCandy[i].height)) {
            rareCandy[i].active = 0;
            rareCandiesCollected++;
            shadowOAM[rareCandy[i].oamIndex].attr0 = ATTR0_HIDE;

            if (level == 2 && rareCandiesCollected == 6) {
                evolution = 2; // Charizard
            }
        }
    }
}

void drawRareCandy() {
    for (int i = 0; i < MAXRARECANDY; i++) {
        if (rareCandy[i].active) {
            shadowOAM[rareCandy[i].oamIndex].attr0 = ATTR0_Y(rareCandy[i].y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
            shadowOAM[rareCandy[i].oamIndex].attr1 = ATTR1_X(rareCandy[i].x - hOff) | ATTR1_SMALL;
            shadowOAM[rareCandy[i].oamIndex].attr2 = ATTR2_TILEID(10, 6);
        }
    }
}

int exit1() {
    int playerX = player.x / 8;
    int playerY = player.y / 8;

    int exit1Tiles[] = {86, 87, 92, 93, 97, 98};

    for (int i = 0; i < 6; i++) {
        if (SCREENBLOCK[28].tilemap[OFFSET(playerX, playerY, 32)] == exit1Tiles[i]) {
            return 1; // At exit
        }
    }
    return 0;
}

int exit2() {
    int playerX = player.x / 8;
    int playerY = player.y / 8;

    int exit2Tiles[] = {32, 33};

    for (int i = 0; i < 2; i++) {
        if (SCREENBLOCK[28].tilemap[OFFSET(playerX, playerY, 32)] == exit2Tiles[i]) {
            return 1; // At exit
        }
    }
    return 0;
}

void initHeal() {
    healItem.width = 16;
    healItem.height = 16;
    healItem.x = 120;
    healItem.y = 128;
    healItem.active = 1;
    healItem.oamIndex = 50;
}

void updateHeal() {
    if (healItem.active && collision(player.x, player.y, player.width, player.height,
        healItem.x, healItem.y, healItem.width, healItem.height)) {
        healItem.active = 0;
        if (lives < 3) {
            lives++;
        }
        shadowOAM[healItem.oamIndex].attr0 = ATTR0_HIDE;
    }
}

void drawHeal() {
    if (healItem.active) {
        shadowOAM[healItem.oamIndex].attr0 = ATTR0_Y(healItem.y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[healItem.oamIndex].attr1 = ATTR1_X(healItem.x - hOff) | ATTR1_SMALL;
        shadowOAM[healItem.oamIndex].attr2 = ATTR2_TILEID(2, 6);
    }
}

void drawHearts() {
    for (int i = 0; i < lives; i++) {
        shadowOAM[lifeCount.oamIndex + i].attr0 = ATTR0_Y(lifeCount.y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[lifeCount.oamIndex + i].attr1 = ATTR1_X(lifeCount.x + i * 10 - hOff) | ATTR1_SMALL;
        shadowOAM[lifeCount.oamIndex + i].attr2 = ATTR2_TILEID(4, 6);
    }
    for (int i = lives; i < 3; i++) {
        shadowOAM[lifeCount.oamIndex + i].attr0 = ATTR0_HIDE;
    }
}