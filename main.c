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
#include "background3.h"
#include "tileset3.h"
#include "background4.h"
#include "tileset4.h"

// State .h files
#include "start.h"
#include "instructions.h"
#include "instructions2.h"
#include "instructions3.h"
#include "instructions4.h"
#include "instructions5.h"
#include "game.h"
#include "pause.h"
#include "digitalSound.h"
#include "themeSong.h"
#include "lrSong.h"
#include "pgSong.h"
#include "gymSong.h"
#include "battleSong.h"
#include "winSong.h"
#include "clouds.h"
#include "pokemon.h"

// Sprite variables
OBJ_ATTR shadowOAM[128];
SPRITE player;

// Surrogate variables
int hOff;
int vOff;
int cloudOff;
int pokemonOff;

// Variables
unsigned short oldButtons;
unsigned short buttons;
int state;
int prevState;

// State enum
enum {
    START,
    GAME,
    PAUSE,
    WIN,
    INSTRUCTIONS, 
    BATTLE
};

// Prototype functions
void initialize();
void update();
void draw();

void goToStart();
void goToInstructions();
void goToGame();
void goToGame2();
void goToGame3();
void goToPause();
void goToWin();

void start();
void game();
void instructions();
void pause();
void win();

void battle();

void clearRareCandy() {
    for (int i = 0; i < MAXRARECANDY; i++) {
        rareCandy[i].active = 0;
        shadowOAM[rareCandy[i].oamIndex].attr0 = ATTR0_HIDE;
    }
}

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
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case BATTLE:
                battle();
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
    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    goToStart();
}

void goToStart() {
    stopSounds();
    initGame();

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    DMANow(3, startPal, BG_PALETTE, 256);
    DMANow(3, startTiles, &CHARBLOCK[0], startTilesLen/2);
    DMANow(3, startMap, &SCREENBLOCK[31], startMapLen/2);
    
    waitForVBlank();
    
    DMANow(3, shadowOAM, OAM, 512);

    setupSounds();
    playSoundA(themeSong_data, themeSong_length, 1);

    state = START;
}

void start(){
    if (BUTTON_PRESSED(BUTTON_START)) {
        initGame();
        goToGame();
    }
    if (BUTTON_PRESSED(BUTTON_RSHOULDER)) {
            goToInstructions();
    }
}

void goToInstructions() {
    hOff = 0;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    prevState = state;

    if (state == START) {
        DMANow(3, instructionsPal, BG_PALETTE, instructionsPalLen / 2);
        DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen / 2);
        DMANow(3, instructionsMap, &SCREENBLOCK[31], instructionsMapLen / 2);
    } else if (level == 1) {
        DMANow(3, instructions2Pal, BG_PALETTE, instructions2PalLen / 2);
        DMANow(3, instructions2Tiles, &CHARBLOCK[0], instructions2TilesLen / 2);
        DMANow(3, instructions2Map, &SCREENBLOCK[31], instructions2MapLen / 2);
    } else if (level == 2) {
        DMANow(3, instructions3Pal, BG_PALETTE, instructions3PalLen / 2);
        DMANow(3, instructions3Tiles, &CHARBLOCK[0], instructions3TilesLen / 2);
        DMANow(3, instructions3Map, &SCREENBLOCK[31], instructions3MapLen / 2);
    } else if (level == 3) {
        DMANow(3, instructions4Pal, BG_PALETTE, instructions4PalLen / 2);
        DMANow(3, instructions4Tiles, &CHARBLOCK[0], instructions4TilesLen / 2);
        DMANow(3, instructions4Map, &SCREENBLOCK[31], instructions4MapLen / 2);
    } else if (level == 4) {
        DMANow(3, instructions5Pal, BG_PALETTE, instructions5PalLen / 2);
        DMANow(3, instructions5Tiles, &CHARBLOCK[0], instructions5TilesLen / 2);
        DMANow(3, instructions5Map, &SCREENBLOCK[31], instructions5MapLen / 2);
    } 

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 512);

    state = INSTRUCTIONS;
}

void instructions() {
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        switch(prevState) {
            case START:
                goToStart();
                break;
            case GAME:
                resume();
                break;
            case PAUSE:
                resume();
                break;
            case BATTLE:
                resume();
                break;
        }
        
    }
}

void goToGame() {
    stopSounds();

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    DMANow(3, tilesetPal, BG_PALETTE, tilesetPalLen / 2);
    DMANow(3, tilesetTiles, &CHARBLOCK[0], tilesetTilesLen/2);
    DMANow(3, background1Map, &SCREENBLOCK[28], background1Len/2);
    
    waitForVBlank();
    
    DMANow(3, shadowOAM, OAM, 512);

    pauseSounds();
    playSoundA(lrSong_data, lrSong_length, 1);

    state = GAME;
}

void goToGame2() {
    stopSounds();

    clearRareCandy();
    
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

    pauseSounds();
    playSoundA(pgSong_data, pgSong_length, 1);

    state == GAME;
}

void goToGame3() {
    DMANow(3, &spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2); 
    DMANow(3, &spritesheetPal, SPRITE_PAL, spritesheetPalLen/2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);
    
    stopSounds();
    
    hideSprites();
    clearRareCandy();

    level = 3;
    rareCandiesCollected = 0;

    initPlayer();
    initBrock();
    // initDialogue();

    player.x = 130;
    player.y = 235;

    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    DMANow(3, tileset3Pal, BG_PALETTE, tileset3PalLen / 2);
    DMANow(3, tileset3Tiles, &CHARBLOCK[0], tileset3TilesLen/2);
    DMANow(3, background3Map, &SCREENBLOCK[28], background3Len/2);

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 512);

    pauseSounds();
    playSoundA(gymSong_data, gymSong_length, 1);

    state = GAME;
}

void battle() {
    updateBattle();

    if (state == BATTLE) {
        drawBattle();
    }

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
}

void goToBattle() {
    stopSounds();

    hideSprites();
    clearRareCandy();

    hOff = 0;
    vOff = 0;

    level = 4;

    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    DMANow(3, tileset4Pal, BG_PALETTE, tileset4PalLen / 2);
    DMANow(3, tileset4Tiles, &CHARBLOCK[0], tileset4TilesLen/2);
    DMANow(3, background4Map, &SCREENBLOCK[28], background4Len/2);

    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    initBattle();

    pauseSounds();
    playSoundA(battleSong_data, battleSong_length, 1);

    state = BATTLE;

}

void game(){
    updateGame();
    drawGame();

    // Debug
    // if (BUTTON_PRESSED(BUTTON_RSHOULDER)) {
    //     goToGame2();
    // }
    if (BUTTON_PRESSED(BUTTON_LSHOULDER)) {
        goToGame3();
    }

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
    
    if (level == 1) {
        if (rareCandiesCollected >= 3 && exit1()) {
            evolution = 1;
            fireballsRemaining = 8;
            goToGame2();
        }
    } else if (level == 2) {
        if (rareCandiesCollected >= 3 && exit2()) {
            evolution = 2;
            goToGame3();
        }
    } else if (level == 3) {
        // updateBrock();
        // if (collisionBrock()) {
        //     startDialogue("I'm Brock, let's battle when you're ready.");
        // }

    }
}

void resume() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    switch (level) {
        case 1: //if curr state is not pause, then reset the tiles, if not then keep tiles
        // Move pause into separate background and slap it on (controls dont work anymore)
            DMANow(3, tilesetPal, BG_PALETTE, tilesetPalLen / 2);
            DMANow(3, tilesetTiles, &CHARBLOCK[0], tilesetTilesLen/2);
            DMANow(3, background1Map, &SCREENBLOCK[31], background1Len/2);
            break;
        case 2:
            DMANow(3, tileset2Pal, BG_PALETTE, tileset2PalLen / 2);
            DMANow(3, tileset2Tiles, &CHARBLOCK[0], tileset2TilesLen/2);
            DMANow(3, background2Map, &SCREENBLOCK[31], background2Len/2);
            break;
        case 3:
            DMANow(3, tileset3Pal, BG_PALETTE, tileset3PalLen / 2);
            DMANow(3, tileset3Tiles, &CHARBLOCK[0], tileset3TilesLen/2);
            DMANow(3, background3Map, &SCREENBLOCK[31], background3Len/2);
            break;
        case 4:
            DMANow(3, tileset4Pal, BG_PALETTE, tileset4PalLen / 2);
            DMANow(3, tileset4Tiles, &CHARBLOCK[0], tileset4TilesLen/2);
            DMANow(3, background4Map, &SCREENBLOCK[31], background4Len/2);
            break;
    }

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 512);

    if (level == 4) {
        state = BATTLE;
    } else {
        state = GAME;
    }
}

void goToPause() {
    hOff = 256;
    vOff = 0;

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;

    DMANow(3, pausePal, BG_PALETTE, pausePalLen / 2);
    DMANow(3, pauseTiles, &CHARBLOCK[0], pauseTilesLen/2);
    DMANow(3, pauseMap, &SCREENBLOCK[31], pauseMapLen/2);

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;

    state = PAUSE;
}

void pause(){
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    if (BUTTON_PRESSED(BUTTON_START)) {
        resume();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        initialize();
    }
    if (BUTTON_PRESSED(BUTTON_RSHOULDER)) {
        goToInstructions();
    }
}

void goToWin() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | BG_ENABLE(1) | SPRITE_ENABLE;
    vOff = 0;
    REG_BG0VOFF = vOff;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(31) | BG_SIZE_SMALL;
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_SIZE_SMALL;

    DMANow(3, cloudsPal, BG_PALETTE, cloudsPalLen / 2);
    DMANow(3, cloudsTiles, &CHARBLOCK[0], cloudsTilesLen/2);
    DMANow(3, cloudsMap, &SCREENBLOCK[31], cloudsMapLen/2);

    DMANow(3, pokemonPal, BG_PALETTE, pokemonPalLen / 2);
    DMANow(3, pokemonTiles, &CHARBLOCK[1], pokemonTilesLen/2);
    DMANow(3, pokemonMap, &SCREENBLOCK[30], pokemonMapLen/2);

    pauseSounds();
    playSoundA(winSong_data, winSong_length, 1);

    state = WIN;
}      

void win(){
    waitForVBlank();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 512);

    cloudOff += 3;
    pokemonOff += 1;
    REG_BG0HOFF = cloudOff;
    REG_BG1HOFF = pokemonOff;

    if (BUTTON_PRESSED(BUTTON_START)) {
        initialize();
    }
}