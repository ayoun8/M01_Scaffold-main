#include "gba.h"
#include "mode0.h"
#include "sprites.h"
#include "print.h"
#include "game.h"

// .h files from tiled/usenti
#include "spritesheet.h"
#include "collisionMap.h"
#include "background1.h"
#include "tileset.h" 
#include "background2.h"
#include "tileset2.h"

// State .h files
#include "start.h"
#include "instructions.h"
#include "game.h"
#include "pause.h"
#include "win.h"
#include "lose.h"

// Sprite variables
OBJ_ATTR shadowOAM[128];
SPRITE player;

// Surrogate variables
int hOff;
int vOff;

// Variables
unsigned short oldButtons;
unsigned short buttons;
int state;

// State enum
enum {
    START,
    GAME,
    //GAME2,
    PAUSE,
    WIN,
    LOSE,
    INSTRUCTIONS
};

// Prototype functions
void initialize();
void update();
void draw();

void goToStart();
void goToInstructions();
void goToGame();
void goToGame2();
void goToPause();
void goToWin();
void goToLose();

void start();
void game();
// void game2();
void instructions();
void pause();
void win();
void lose();

int main() {
    initialize();

    while(1) {
        oldButtons = buttons;
        buttons = REG_BUTTONS;

         switch (state) {
            case START:
                start();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case GAME:
                game();
                break;
            // case GAME2:
            //     break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
         }
    }
    return 0;
}

void initialize() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;

    // Load sprites into memory
    DMANow(3, &spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2); 
    DMANow(3, &spritesheetPal, SPRITE_PAL, spritesheetPalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    // Initialize hOff and vOff
    hOff = 0;
    vOff = 0;

    goToStart();
}

void goToStart() {
    initGame();

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, startPal, BG_PALETTE, 256);
    DMANow(3, startTiles, &CHARBLOCK[0], startTilesLen/2);
    DMANow(3, startMap, &SCREENBLOCK[31], startMapLen/2);
    
    waitForVBlank();
    
    DMANow(3, shadowOAM, OAM, 512);

    state = START;
}

void start(){
    if (BUTTON_PRESSED(BUTTON_START)) {
        initGame();
        goToGame();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToInstructions();
    }
}

void goToInstructions() {
    hOff = 0;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, instructionsPal, BG_PALETTE, instructionsPalLen / 2);
    DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen / 2);
    DMANow(3, instructionsMap, &SCREENBLOCK[31], instructionsMapLen / 2);

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 512);

    state = INSTRUCTIONS;
}

void instructions() {
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToGame() {
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    DMANow(3, tilesetPal, BG_PALETTE, tilesetPalLen / 2);
    DMANow(3, tilesetTiles, &CHARBLOCK[0], tilesetTilesLen/2);
    DMANow(3, background1Map, &SCREENBLOCK[28], background1Len/2);
    
    waitForVBlank();
    
    DMANow(3, shadowOAM, OAM, 512);

    state = GAME;
}

void goToGame2() {
    level = 2;
    rareCandiesCollected = 0;
    fireballsRemaining = 5;
    initRareCandy();
    initPlayer();
    initFireballs();
    initHeal();

    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    DMANow(3, tileset2Pal, BG_PALETTE, tileset2PalLen / 2);
    DMANow(3, tileset2Tiles, &CHARBLOCK[0], tileset2TilesLen/2);
    DMANow(3, background2Map, &SCREENBLOCK[28], background2Len/2);

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 512);
}

void game(){
    updateGame();
    waitForVBlank();
    drawGame();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
    if (gameLost) {
        goToLose();
    }
    
    if (level == 1) {
        if (rareCandiesCollected >= 3 && exit1()) {
            evolution = 1;
            fireballsRemaining = 8;
            goToGame2();
        }
    }
    // } else if (level == 2) {
    //     if (rareCandiesCollected == 3 && exit2()) {
    //         evolution = 2;
    //         goToGame3();
    //     }
    // }
}

void goToPause() {
    hOff = 256;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, pausePal, BG_PALETTE, pausePalLen / 2);
    DMANow(3, pauseTiles, &CHARBLOCK[0], pauseTilesLen/2);
    DMANow(3, pauseMap, &SCREENBLOCK[31], pauseMapLen/2);

    state = PAUSE;
}

void pause(){
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToStart();
    }
}

void goToWin() {
    hOff = 256;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, winPal, BG_PALETTE, winPalLen / 2);
    DMANow(3, winTiles, &CHARBLOCK[0], winTilesLen/2);
    DMANow(3, winMap, &SCREENBLOCK[31], winMapLen/2);

    state = WIN;
}      

void win(){
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToLose() {
    hOff = 256;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, losePal, BG_PALETTE, losePalLen / 2);
    DMANow(3, loseTiles, &CHARBLOCK[0], loseTilesLen/2);
    DMANow(3, loseMap, &SCREENBLOCK[31], loseMapLen/2);

    state = LOSE;
}

void lose(){
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}