#include "dialog.h"
#include "mode0.h"
#include "sprites.h"

#define FONT_OAM_START 60
#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8

DialogBox dialogBox;

void initDialogSystem() {
    dialogBox.active = 0;
}

void startDialog(const char* message, int pauseGame, int wait, int duration) {
    dialogBox.active = 1;
    dialogBox.message = message;
    dialogBox.pauseGame = pauseGame;
    // Wait for user input to proceed (A)
    dialogBox.wait = wait;
    // Can use to make dialog automatically close after duration
    dialogBox.timer = duration;
    // Gives effect of typing (counts up from 0-num char in index)
    dialogBox.currCharIndex = 0;
    // Delay between appearance of characters
    dialogBox.charDelay = 2;
    dialogBox.charDelayCounter = 0;

    dialogBox.x = 10;
    dialogBox.y = 130;
    dialogBox.width = 22;
    dialogBox.height = 4;
}

void startMultiDialog(const char* messages[], int count, int pauseGame, int wait, int duration) {
    dialogBox.active = 1;
    dialogBox.pauseGame = pauseGame;
    dialogBox.wait = wait;
    dialogBox.timer = duration;

    // Total num messages
    dialogBox.numMessages = count;
    // Set current message to 0
    dialogBox.currentMessage = 0;

    // Array of messages
    for (int i = 0; i < count; i++) {
        dialogBox.messages[i] = messages[i];
    }

    // Set curr message to first one in array
    dialogBox.message = dialogBox.messages[0];
    dialogBox.currCharIndex = 0;
    dialogBox.charDelay = 2;
    dialogBox.charDelayCounter = 0;
    
    dialogBox.x = 10;
    dialogBox.y = 130;
    dialogBox.width = 22;
    dialogBox.height = 4;
}

void updateDialog() {
    // If dialog is not active, return
    if (!dialogBox.active) {
        return;
    }
    if (dialogBox.wait) {
        if (BUTTON_PRESSED(BUTTON_RSHOULDER)) {
            // Attempt
            if (dialogBox.currCharIndex < strlen(dialogBox.message)) {
                // Skip to end of message
                dialogBox.currCharIndex = strlen(dialogBox.message);
            } else {
                // Go to next message
                if (dialogBox.currentMessage + 1 < dialogBox.numMessages) {
                    dialogBox.currentMessage++;
                    dialogBox.message = dialogBox.messages[dialogBox.currentMessage];
                    dialogBox.currCharIndex = 0;
                } else {
                    closeDialog();
                }
            }
            // Go to next message WITH RSHOULDER
            if (dialogBox.currentMessage + 1 < dialogBox.numMessages) {
                dialogBox.currentMessage++;
                dialogBox.message = dialogBox.messages[dialogBox.currentMessage];
                dialogBox.currCharIndex = 0;
            } else {
                closeDialog();
            }
        }
    } else if (dialogBox.timer > 0) {
        dialogBox.timer--;
        if (dialogBox.timer == 0) {
            closeDialog();
        }
    }
    // Typing effect- increase curr char index every 2 time intervals
    dialogBox.charDelayCounter++;
    if (dialogBox.charDelayCounter >= dialogBox.charDelay) {
        dialogBox.charDelayCounter = 0;
        dialogBox.currCharIndex++;
    }
}

void drawDialog() {
    if (!dialogBox.active) {
        return;
    }

    int letterIndex = FONT_OAM_START;
    int newLine = 0;

    // Loops through characters to draw in the message
    for (int i = 0; i < dialogBox.currCharIndex && dialogBox.message[i] != '\0'; i++) {    
        int row = 0;
        int col = 0;

        // Loops through the curr message chars
        // This is to make sure all new lines are being registered
        // Check for #, then the col against box width to make dialog wrap and not go out of the box, then it creates sprites for the characters from line 130
        for (int j = 0; j < i; j++) {
            char c = dialogBox.message[j];
        
            // If hashtag, force new line
            if (c == '#') {
                // Increment y position
                row++;
                // Stay in x position
                col = 0;
                continue;
            }
            // Forcing a new line when you get to the end
            if (col >= dialogBox.width) {
                // Go back to front of dialog box then go down to start a new line
                col = 0;
                row++;
            }
            // Increment x so you don't draw all chars in same spot
            col++;
        }

        char ch = dialogBox.message[i];

        // Only sprites from a to z
        if (ch >= 'a' && ch <= 'z') {
            // Makes it capital
            ch -= 32;
        }

        // If detect space or #, don't draw sprite, leave an empty space basically
        // # in line 111 is to make # force a new line
        // The # in line 137 is to skip sprite creation
        if (ch == ' ' || ch == '#') {
            continue;
        }

        int tileCol = 25;
        if (ch >= 'A' && ch <= 'Z') {
            // A = 65, need to make our A = 65
            // 65 needs to equal 0 since that is the x location of the A sprite on the spritesheet
            // Maps ascii characters to spritesheet
            tileCol = ch - 'A';
        }

        // Row in spritesheet
        int tileRow = 25;

        // Position to draw the character
        // -4 for padding to make sure it fits into the box nicely
        int x = dialogBox.x + col * FONT_CHAR_WIDTH - 4;
        int y = dialogBox.y + row * FONT_CHAR_HEIGHT - 4;

        shadowOAM[letterIndex].attr0 = ATTR0_Y(y) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[letterIndex].attr1 = ATTR1_X(x) | ATTR1_TINY;
        shadowOAM[letterIndex].attr2 = ATTR2_TILEID(tileCol, tileRow);

        letterIndex++;
    }

    // Erases the previous characters
    // Cleaner way to do this to prevent flashing
    for (int i = letterIndex; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }

    // Draw dialog box background
    int boxIndex = 120;
    // Each chunk is 4 tiles = 64px wide
    int tilesPerChunk = 4;
    // 15 tiles wide
    int chunks = 15 / tilesPerChunk;

    // Draws dialog background, hard coded at 6 to create 6 tiles along (the width)
    for (int i = 0; i < 6; i++) {
        shadowOAM[boxIndex + i].attr0 = ATTR0_Y(dialogBox.y - 8) | ATTR0_SQUARE | ATTR0_4BPP;
        shadowOAM[boxIndex + i].attr1 = ATTR1_X(dialogBox.x - 8 + i * 32) | ATTR1_MEDIUM;
        shadowOAM[boxIndex + i].attr2 = ATTR2_TILEID(i * 4, 26);
    }
}

// Closes dialog (hides all characters and disables dialog box)
void closeDialog() {
    dialogBox.active = 0;
    for (int i = FONT_OAM_START; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }
}