#include "gba.h"
#include "sprites.h"

// Constant
#define MAXRARECANDY 6
#define MAXFIREBALLS 5
#define MAXFIRES 6
int candyX[MAXRARECANDY];
int candyY[MAXRARECANDY];

#define SCREENWIDTH 240
#define SCREENHEIGHT 160
#define MAPWIDTH 256
#define MAPHEIGHT 256

// External variables
extern SPRITE player;
extern SPRITE fireballs[MAXFIREBALLS];
extern SPRITE rareCandy[MAXRARECANDY];
extern SPRITE healItem;
extern SPRITE lifeCount;
extern SPRITE blaze[MAXFIRES];
extern SPRITE brock;
extern OBJ_ATTR shadowOAM[128];

extern int hOff;
extern int vOff;
extern int lives;
extern int isPokemon;
extern int evolution;
extern int rareCandiesCollected;
extern int fireballsRemaining;
extern int gameLost;
extern int level;

// Function prototypes
void initGame();
void updateGame();
void drawGame();

void initPlayer();
void updatePlayer();
void drawPlayer();

void initFireballs();
void placeFireball();
void updateFireballs();
void drawFireballs();

void initRareCandy();
void updateRareCandy();
void drawRareCandy();

void findFlowerSpots();
void randomFlowers();
void revealCandy();
void burn(int x, int y);

void initBlaze();
void updateBlaze();
void drawBlaze();

void initHeal();
void updateHeal();
void drawHeal();

void drawHearts();

// void drawWin();
// void drawLose();
// void drawStart();
// void drawPause();
// void drawInstructions();

void drawGame1();
// void drawGame2();
// void drawGame3();

void initBrock();
void updateBrock();
void drawBrock();