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
#include "collisionMap3.h"
#include "background3.h"
#include "tileset3.h"
#include "collisionMap4.h"
#include "background4.h"
#include "tileset4.h"

#include "digitalSound.h"
#include "burnSound.h"
#include "roarSound.h"

// Global variable definitions
SPRITE player;
SPRITE fireballs[MAXFIREBALLS];
SPRITE rareCandy[MAXRARECANDY];
SPRITE blaze[MAXFIRES];
SPRITE healItem;
SPRITE lifeCount;
SPRITE brock;
SPRITE charizard;
SPRITE onyx;

OBJ_ATTR shadowOAM[128];

// Direction enum
typedef enum {
    DOWN,
    UP,
    LEFT,
    RIGHT
} DIRECTION;

typedef enum {
    IDLE,
    TALKING,
    MOVING
} BROCKSTATE;

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

// int gameLost = 0;
int level = 1;

int charizardHP;
int onyxHP;
int charizardDamaged;
int onyxDamaged;
int charizardTurn;
int move;
int attackCooldown;

int onyxTimer;
int charizardTimer;

BROCKSTATE brockState;

inline unsigned char colorAt(int x, int y){
    if (level == 1) {
        return ((unsigned char *) collisionMapBitmap) [OFFSET(x, y, MAPWIDTH)];
    } else if (level == 2) {
        return ((unsigned char *) collisionMap2Bitmap) [OFFSET(x, y, MAPWIDTH)];
    } else if (level == 3) {
        return ((unsigned char *) collisionMap3Bitmap) [OFFSET(x, y, MAPWIDTH)];
    } else if (level == 4){
        return ((unsigned char *) collisionMap4Bitmap) [OFFSET(x, y, MAPWIDTH)];
    }
}

void initGame() {
    initPlayer();
    initFireballs();
    initRareCandy();
    initHeal();
    initBlaze();

    lives = 3;
    fireballsRemaining = 5;
    rareCandiesCollected = 0;
    evolution = 0;
    isPokemon = 0;
    level = 1;

    lifeCount.x = 45;
    lifeCount.y = 65;
    lifeCount.width = 16;
    lifeCount.height = 16;
    lifeCount.oamIndex = 18;
}

void updateGame() {
    updatePlayer();
    updateFireballs();
    updateRareCandy();
    updateHeal();
    updateBlaze();

    if (level == 1 && rareCandiesCollected == 3) {
        evolution = 1;
        if (exit1()) {
            fireballsRemaining = 5;
            fireballDelayTimer = 60;
            goToGame2();
        }
    }

    if (level == 2) {
        if (rareCandiesCollected == 3 && exit2()) {
            evolution = 2;
            goToGame3();
        }
    }
    if (level == 3) {
        updateBrock();
    }
    if (level == 4) {
        updateBattle();
    }
}

void drawGame() {
    drawPlayer();
    drawFireballs();
    drawRareCandy();
    drawBlaze();

    if (level == 2) {
        drawHeal();
    }
    if (level == 3) {
        drawBrock();
    }
    if (level == 4) {
        drawBattle();
    }
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

    int tileX = (player.x + player.width / 2) / 8;
    int tileY = (player.y + player.height / 2) / 8;

    unsigned short tile = SCREENBLOCK[28].tilemap[OFFSET(tileX, tileY, 32)];

    if (level == 1) {
        if (tile == 39 || tile == 40 || tile == 47 || tile == 48) {
            if (fireballsRemaining < 2) {
                fireballsRemaining += MAXFIREBALLS;
                if (fireballsRemaining > 5) {
                    fireballsRemaining = 5;
                }
            }
        }
    }
    if (level == 2) {
        if (tile == 44 || tile == 45 || tile == 46 || tile == 47) {
            if (fireballsRemaining < 2) {
                fireballsRemaining += MAXFIREBALLS;
                if (fireballsRemaining > 5) {
                    fireballsRemaining = 5;
                }
            }
        }
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

        if (evolution == 0) {
            shadowOAM[0].attr2 = ATTR2_TILEID(player.direction * 2 + 8, player.currentFrame * 2);
        } else if (evolution == 1) {
            shadowOAM[0].attr2 = ATTR2_TILEID(player.direction * 2 + 16, player.currentFrame * 2);
        } else if (evolution == 2) {
            shadowOAM[0].attr2 = ATTR2_TILEID(player.direction * 2 + 24, player.currentFrame * 2);
        }
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

    playSoundB(burnSound_data, burnSound_length, 0);

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

        if (level == 1) {
            topLeftY += 1;
        }

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
            if (level == 1) {
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY, 32)] = 2;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY, 32)] = 2;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY + 1, 32)] = 2;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY + 1, 32)] = 2;

                for (int i = 0; i < MAXFIRES; i++) {
                    if (!blaze[i].active) {
                        blaze[i].x = topLeftX * 8;
                        blaze[i].y = topLeftY * 8;
                        blaze[i].active = 1;
                        blaze[i].timer = 20;
                        blaze[i].currentFrame = 0;
                        break;
                    }
                }
            } else if (level == 2) {
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY, 32)] = 8;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY, 32)] = 8;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX, topLeftY + 1, 32)] = 9;
                SCREENBLOCK[28].tilemap[OFFSET(topLeftX + 1, topLeftY + 1, 32)] = 9;

                for (int i = 0; i < MAXFIRES; i++) {
                    if (!blaze[i].active) {
                        blaze[i].x = topLeftX * 8;
                        blaze[i].y = topLeftY * 8;
                        blaze[i].active = 1;
                        blaze[i].timer = 20;
                        blaze[i].currentFrame = 0;
                        break;
                    }
                }
            } else {
                return 0;
            }

            for (int j = 0; j < MAXRARECANDY; j++) {
                // Tiles
                int candyX = rareCandy[j].x / 8;
                int candyY = rareCandy[j].y / 8;
                
                if (!rareCandy[j].active &&
                    // candyX == topLeftX &&
                    // candyY == topLeftY) {
                    // rareCandy[j].active = 1;
                    abs(candyX - topLeftX) <= 2 &&
                    abs(candyY - topLeftY) <= 2) {
                    rareCandy[j].active = 1;
                }
            }

            // Recharge fireballs
            if ((topLeft == 39) || (topRight == 40) || (bottomLeft == 47) || (bottomRight == 48)) {
                // Change to add 5
                fireballsRemaining++;
            }
            //break; // only burn one
        }
    }
}

void initBlaze() {
    for (int i = 0; i < MAXFIRES; i++) {
        blaze[i].width = 16;
        blaze[i].height = 16;
        blaze[i].active = 0;
        blaze[i].oamIndex = 60 + i;
        blaze[i].timer = 0;
    }
}

void updateBlaze() {
    for (int i = 0; i < MAXFIRES; i++) {
        if (blaze[i].active) {
            blaze[i].timer--;
            if (blaze[i].timer <= 0) {
                blaze[i].active = 0;
                shadowOAM[blaze[i].oamIndex].attr0 = ATTR0_HIDE;
            } else {
                blaze[i].currentFrame = (20 - blaze[i].timer) / 10;
            }
        }
    }
}

void drawBlaze() {
    for (int i = 0; i < MAXFIRES; i++) {
        if (blaze[i].active) {
            shadowOAM[blaze[i].oamIndex].attr0 = ATTR0_Y(blaze[i].y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
            shadowOAM[blaze[i].oamIndex].attr1 = ATTR1_X(blaze[i].x - hOff) | ATTR1_SMALL;
            shadowOAM[blaze[i].oamIndex].attr2 = ATTR2_TILEID(6 + blaze[i].currentFrame * 2, 6);
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
            rareCandy[i].active = 0;
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
            rareCandy[i].active = 0;
            rareCandy[i].oamIndex = 30 + i;
        }
    }
}

void updateRareCandy() {
    for (int i = 0; i < MAXRARECANDY; i++) {
        if (rareCandy[i].active && collision(player.x, player.y, player.width, player.height,
            rareCandy[i].x, rareCandy[i].y, rareCandy[i].width, rareCandy[i].height)) {
            rareCandy[i].active = 0;
            rareCandiesCollected++;
            shadowOAM[rareCandy[i].oamIndex].attr0 = ATTR0_HIDE;

            if (level == 2 && rareCandiesCollected == 3) {
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

    if (level == 2) {
        healItem.x = 120;
        healItem.y = 128;
    }
    
    healItem.active = 1;
    healItem.oamIndex = 60;
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
        shadowOAM[lifeCount.oamIndex + i].attr0 = ATTR0_Y(lifeCount.y) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[lifeCount.oamIndex + i].attr1 = ATTR1_X(lifeCount.x + i * 10) | ATTR1_SMALL;
        shadowOAM[lifeCount.oamIndex + i].attr2 = ATTR2_TILEID(4, 6);
    }
    for (int i = lives; i < 3; i++) {
        shadowOAM[lifeCount.oamIndex + i].attr0 = ATTR0_HIDE;
    }
}

void initBrock() {
    brock.x = 120;
    brock.y = 150;
    brock.width = 16;
    brock.height = 16;
    brock.oamIndex = 80;
    brock.speed = 1;
    brockState = IDLE;
}

void updateBrock() {
    if (brockState == IDLE) {
        if (collision(player.x, player.y, player.width, player.height, brock.x, brock.y, brock.width, brock.height)) {
            goToBattle();
        }
    } else if (brockState == TALKING) {
        if (BUTTON_PRESSED(BUTTON_A)) {
            brockState = MOVING;
        }
    } else if (brockState == MOVING) {
        updateBrockMovement();
        if (brock.x < player.x) {
            brock.x += brock.speed;
        } else if (brock.x > player.x) {
            brock.x -= 1;
        }
    }
}

void finishDialogue() {
    brockState = MOVING;
}

void updateBrockMovement() {
    // int doorX = 3;
    // int doorY = 3;

    // if (brockState == MOVING) {
    //     if (brock.x < doorX) {
    //         brock.x += brock.speed;
    //     } else if (brock.x > doorX) {
    //         brock.x -= brock.speed;
    //     }
    //     if (brock.y < doorY) {
    //         brock.y += brock.speed;
    //     } else if (brock.y > doorY) {
    //         brock.y -= brock.speed;
    //     }

    //     if (abs(brock.x - doorX) < brock.speed && abs(brock.y - doorY) < brock.speed) {
    //         brock.x = doorX;
    //         brock.y = doorY;
    //         // Transition 
    //         brockState = IDLE;
    //     }
    // }
}

void drawBrock() {
    if (brockState == IDLE) {
        shadowOAM[brock.oamIndex].attr2 = ATTR2_TILEID(0, 8);
    } else if (brockState == TALKING) {
        shadowOAM[brock.oamIndex].attr2 = ATTR2_TILEID(2, 8);
    } else if (brockState == MOVING) {
        shadowOAM[brock.oamIndex].attr2 = ATTR2_TILEID(4, 8);
    }

    shadowOAM[brock.oamIndex].attr0 = ATTR0_Y(brock.y - vOff) | ATTR0_SQUARE | ATTR0_4BPP;
    shadowOAM[brock.oamIndex].attr1 = ATTR1_X(brock.x - hOff) | ATTR1_SMALL;
}

void initBattle() {
    charizard.x = 20;
    charizard.y = 80;
    charizard.oamIndex = 5;
    charizard.width = 32;
    charizard.height = 32;
    charizardHP = 5;
    charizardDamaged = 0;

    onyx.x = 140;
    onyx.y = 20;
    onyx.oamIndex = 6;
    onyx.width = 32;
    onyx.height = 32;
    onyxHP = 5;
    onyxDamaged = 0;

    charizardTurn = 1;
    move = 0;
    attackCooldown = 0;
}

void updateBattle() {
    if (attackCooldown > 0) {
        attackCooldown--;
        return;
    }

    if (charizardTurn == 1) {
        if (BUTTON_PRESSED(BUTTON_LEFT)) {
            // Flamethrower
            move = 0;
        } 
        if (BUTTON_PRESSED(BUTTON_RIGHT)) {
            // Scratch
            move = 1;
        }
        if (BUTTON_PRESSED(BUTTON_A)) {
            playSoundB(roarSound_data, roarSound_length, 0);
            // 85% chance of attack landing
            if (rand() % 100 < 85) {
                onyxHP--;
                onyxDamaged = 10;
                onyxTimer = 45;
                SPRITE_PAL[9] = RGB(31, 0, 0);
                SPRITE_PAL[1] = RGB(31, 0, 0);
            }
            charizardTurn = 0;
            attackCooldown = 20;
        }
    } 
    else {
        // Onyx's attack has an 80% chance of landing
        if (attackCooldown <= 0) {
            if (rand() % 100 < 80) {
                charizardHP--;
                charizardDamaged = 10;
                charizardTimer = 45;
                SPRITE_PAL[10] = RGB(31, 0, 0);
                SPRITE_PAL[5] = RGB(31, 0, 0);
            }
            charizardTurn = 1;
            attackCooldown = 20;
        }
        }
        if (charizardHP <= 0) {
            level = 3;
            shadowOAM[charizard.oamIndex].attr0 = ATTR0_HIDE;
            shadowOAM[onyx.oamIndex].attr0 = ATTR0_HIDE;
            goToGame3();
        } else if (onyxHP <= 0) {
            goToWin();
        }
}

void drawBattle() {
    // Draw Charizard
    shadowOAM[charizard.oamIndex].attr0 = ATTR0_Y(charizard.y) | ATTR0_SQUARE | ATTR0_4BPP;
    shadowOAM[charizard.oamIndex].attr1 = ATTR1_X(charizard.x) | ATTR1_LARGE;
    
    if (charizardDamaged > 0) {
        shadowOAM[charizard.oamIndex].attr2 = ATTR2_TILEID(8, 8) | ATTR2_PALROW(0);
        charizardDamaged--;
    } else if (!charizardTurn) {
        shadowOAM[charizard.oamIndex].attr2 = ATTR2_TILEID(8, 8);
    } else {
        shadowOAM[charizard.oamIndex].attr2 = ATTR2_TILEID(16, 8);
    }

    // Draw Onyx
    shadowOAM[onyx.oamIndex].attr0 = ATTR0_Y(onyx.y) | ATTR0_SQUARE | ATTR0_4BPP;
    shadowOAM[onyx.oamIndex].attr1 = ATTR1_X(onyx.x) | ATTR1_LARGE;
    
    if (onyxDamaged > 0) {
        shadowOAM[onyx.oamIndex].attr2 = ATTR2_TILEID(0, 16) | ATTR2_PALROW(0);
        onyxDamaged--;
    } else if (!charizardTurn) {
        shadowOAM[onyx.oamIndex].attr2 = ATTR2_TILEID(8, 16);
    } else {
        shadowOAM[onyx.oamIndex].attr2 = ATTR2_TILEID(0, 16);
    }

    for (int i = 0; i < 5; i++) {
        // Charizard HP
        shadowOAM[20 + i].attr0 = ATTR0_Y(145) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[20 + i].attr1 = ATTR1_X(10 + i * 10) | ATTR1_TINY;
        shadowOAM[20 + i].attr2 = ATTR2_TILEID(4, 6);

        if (i >= charizardHP) {
            shadowOAM[20 + i].attr0 |= ATTR0_HIDE;
        }

        // Onyx HP
        shadowOAM[30 + i].attr0 = ATTR0_Y(5) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[30 + i].attr1 = ATTR1_X(200 - i * 10) | ATTR1_TINY;
        shadowOAM[30 + i].attr2 = ATTR2_TILEID(4, 6);
        if (i >= onyxHP) {
            shadowOAM[30 + i].attr0 |= ATTR0_HIDE;
        }
    }
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    // Restore sprite colors
    if (charizardTimer > 0) {
        charizardTimer--;
        if (charizardTimer == 0) {
            SPRITE_PAL[10] = RGB(31, 13, 9);
            SPRITE_PAL[5] = RGB(7, 7, 15);
        }
    }
    if (onyxTimer > 0) {
        onyxTimer--;
        if (onyxTimer == 0) {
            SPRITE_PAL[9] = RGB(22, 22, 26);
            SPRITE_PAL[1] = RGB(14, 20, 24);
        }
    }
}