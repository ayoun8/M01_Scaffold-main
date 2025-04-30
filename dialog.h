#ifndef DIALOG_H
#define DIALOG_H

#include "gba.h"

#define DIALOG_MAX_LENGTH 128
#define MAX_DIALOG_PAGES 8

typedef struct {
    int active;
    const char* message;
    const char* messages[MAX_DIALOG_PAGES];
    int numMessages;
    int currentMessage;

    int pauseGame;
    int wait;
    int timer;

    int currCharIndex;
    int charDelay;
    int charDelayCounter;

    int x, y;
    int width, height;
} DialogBox;

extern DialogBox dialogBox;

void initDialogSystem();
void startDialog(const char* message, int pauseGame, int wait, int duration);
void updateDialog();
void drawDialog();
void closeDialog();

#endif