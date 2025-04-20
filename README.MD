***What my game has so far***
1. FULL SPRITESHEET
    a. A *trainer* with three frames of animation per direction
    b. *Charmander* with three frames of animation per direction
    c. The fireball, heal, heart, explosions, and rare candy are also done
        i. Fireballs loading when pressing A as a pokemon
        ii. Heals load in level 2 (special item)
        iii. Hearts load in level 3 to show HP
        iv. Explosions load when you burn flowers/grass
        v. Rare candies start initialized for easy level traversing, but also appear after you burn their "hidden" locations.
    d. *Charmeleon* and *Charizard* players with three frames of animation per direction- each loading based on the evolution
    e. *Brock* with three frames of animation per direction
    f. Charizard and Onyx boss battle sprites with two frames of animation
2. FUNCTIONING MOVEMENT
    a. The player can move using the D pad
    b. The pokemon can move using the D pad
3. FUNCTIONING COLLISION MAP
    a. Collision map 1, 2, 3, and 4 work
        i. Cannot walk through buildings, trees, walls
4. FUNCTIONING BUTTONS:
    a. Press A to start
    b. Press SELECT in the start screen to see the ***instructions***
    c. Press START once in game to pause
        - Press SELECT in the pause menu to go back to start
    d. IN LEVEL 1: 
        - B button: you can switch between the trainer and the pokemon
        - A button: you can release a fireball *ONLY* as a pokemon
    e. IN LEVEL 2: 
        - B button: you can switch between the trainer and the pokemon
        - A button: you can release a fireball *ONLY* as a pokemon
    f. IN LEVEL 4;
        - LEFT: changes move to flamethrower
        - RIGHT: changes move to scratch
        - A: attack
    DEBUGGING
    g. Press LSHOULDER to go to level 3
5. FUNCTIONING STATES:
    a. *Start, instructions, game, pause, win, lose, battle* states are all implemented
        - ***Winning and losing conditions are also implemented***
            - To win, beat onyx
            - To lose, lose to onyx, respawn to level 3
        - All states are functional.
6. FUNCTIONING TILEMAP AND TILESET FOR ALL FOUR LEVELS
7. LOOPING SONGS FOR EVERY LEVEL
    - Does not mix up with the other sounds
    - Pause is successful (small error with tiles but will fix for final project)
7a. OVERLAPPING SOUND WITH LOOPING SOUND
    - Fire woosh noise when you place a fireball
    - Charizard roar when he attacks
8. INSTRUCTIONS FOR ALL LEVELS
    - Detailed instructions that are accessible from the pause screen with RSHOULDER
9. PARALLAX SCROLLING BACKGROUND FOR WIN SCREEN!
10. SPRITE MODIFICATION- pokemon turn red when hit during the boss battle. 
11. YOU CAN RECHARGE FIREBALLS NOW!!
    - Touch a sign in any level! (There are multiple in level 1 but only 1 in level 2).

***What needs to be added for my final project***
1. I still need to lock down my rare candy initialization
    a. I feel like I can definitely get the hiccups under control by demo day.
    b. They do work, but it's weird. Sometimes they work perfectly and sometimes they spawn slightly off. 
2. Final art:
    a. I need to add dialogue boxes.
    b. I need to add cut scenes.
    c. ^ I actually will revisit these and I am not sure if I want them as a final requirement. I could just keep brock stationary and non interactive. I was hoping to change these into stretch goals. 
        - I am slightly confused on what the requirements were because I wasn't sure what my feedback was. I thought dialogues and scenes were art, so I will add that for a final polish for my final project. 

***Bugs***
1. Rare candy spawn positions aren't as accurate as I want them to be.
2. The pause pauses things well but also regenerates the burnt tiles (but still keeps your progress).
3. ^ I am still working on them. They don't destroy the game play, since the rare candy will still spawn if you hit a nearby coordinate and the pause still pauses the game and keeps the progress. I will clean this up for the final project. 

***NAVIGATION***
a. Press START to start
b. Press SELECT in the start screen to see the ***instructions***
c. Press START once in game to pause
    - Press SELECT in the pause menu to go back to start
DEBUGGING
    d. Press RSHOULDER to go to level 2
    e. Press LSHOULDER to go to level 3

***Notes***
- I made the rare candies initialize unhidden so you don't have to search for them.
    - To see how my game is supposed to work, you can change the "rareCandy[i].active = 0" to "rareCandy[i].active = 1" under //Hide in the function initRareCandy() in game.c. 
- My heal works and loads.
- I am going to add in a SECOND cheat- the thunderbolt that turns you into a pikachu. 
    - I am going to do this in the final project since the M03 instructions said we can save our chat for our final project. 
- You're more likely to lose when you use scratch
- You can access the exits once you're the next evolution. 
- The fireball delay timer could cause you to die if you advance too quickly (because you aren't supposed to normally).

FIREBALL LOCATIONS:


***INSTRUCTIONS FOR MY GAME***
1. Check "navigation" for information on how to use my state machine
2. Check "functioning D pad" for information on how to move (use the D pad)
3. Collect 3 rare candies and go through the collect door as charmelon to advance to the next round. 
4. GAMEPLAY:
    ***LEVEL1***
    - In level 1, you are supposed to burn grass to find hidden rare candies underneath.
        - You can switch between the player and the pokemon by pressing B.
        - You can shoot fireballs *ONLY* as the pokemon by pressing A.
        - The fireballs burn the flowers.
        - Collect 3 rare candies to evolve into Charmeleon and be able to advance to Petalburg through a correct door.
        - You have 5 fireballs. Use them wisely. If you run out before finding all of the rare candy, you have to restart the level (but this won't be an issue yet because I made them all start active for ease). 
        - The candies spawn in the same spot every time you respawn, so this level is ultimately a memorization game. 
        - ***The fireballs need to be placed in the middle of the flowers. This is also a game of precision.***
        - Once you evolve into charmeleon, you can access the exit. It is one of the doors- choose wisely.
    
    ***LEVEL 2***
    - It is the same concept for level 2. Find and collect rare candies then go through the fence to advance to level 3.
      - Collect the heal and lightning bolt, your special item.
      - You can leave out of the fence once you are charizard. 
    
    ***LEVEL 3***
    - Once in level 3, go collide with brock to start the boss battle
    
    ***BOSS BATTLE***
    - In the boss battle, press left for flamethrower and right for scratch.
        - The heal from earlier gives you an extra life once your lives drop below 3. 
        - Press A to attack. Once you attack, Onyx attacks.
        - You both have a chance to miss each other.
        - If you win, it goes to the win screen. If you lose, then you go back to level 3 and have to rebattle Brock.