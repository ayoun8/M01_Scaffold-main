***What my game has so far***
1. FULLS SPRITESHEET
    a. A *trainer* with three frames of animation per direction
    b. *Charmander* with three frames of animation per direction
    c. The fireball, heal, heart, explosions, and rare candy are also done
2. FUNCTIONING MOVEMENT
    a. The player can move using the D pad
    b. The pokemon can move using the D pad
3. FUNCTIONING COLLISION MAP
    a. Cannot walk through buildings
4. FUNCTIONING BUTTONS:
    a. Press A to start
    b. Press SELECT in the start screen to see the ***instructions***
    c. Press START once in game to pause
        - Press SELECT in the pause menu to go back to start
    d. IN GAME: 
        - B button: you can switch between the trainer and the pokemon
        - A button: you can release a fireball *ONLY* as a pokemon
5. FUNCTIONING STATES:
    a. *Start, instructions, game, pause, win, lose* states are all implemented
        - ***Winning and losing conditions are also implemented***
            - To win, just collect all of the rare candies
            - To lose, collect < 3 rare candies and use up all of your fire balls
        - All states are functional.
6. FUNCTIONING TILEMAP AND TILESET FOR LITTLEROOT
    a. I have a fleshed out tilemap and a tileset

***What needs to be added for my next milestone***
1. I need to finish hiding the rare candies for the Littleroot tilemap.
2. I need to create a collison map for Petalburg.
3. I need to add the sprites for Charmeleon and Charizard.

***Bugs***
1. No bugs rn.

***NAVIGATION***
a. Press A to start
b. Press SELECT in the start screen to see the ***instructions***
c. Press START once in game to pause
    - Press SELECT in the pause menu to go back to start
d. IN GAME: 
    - B button: you can switch between the trainer and the pokemon
    - A button: you can release a fireball *ONLY* as a pokemon

***Notes****
- My heal works and loads. I commented out the code for it though since the heal does not appear until Petalburg.
- My hearts work as well. I commented out the code for them because you do not need hp until level 3. 
- I also did not hide the rare candies and just placed them in the center so they are easy to collect to test my win state.

***INSTRUCTIONS FOR MY GAME***
1. Check "navigation" for information on how to use my state machine
2. Check "functioning D pad" for information on how to move (use the D pad)
3. Collect 3 rare candies to win.
4. Have < 3 rare candies and use up all of your fireballs to lose.
5. Gameplay idea: in progress, but this is what it's supposed to look like.
    - In this level, you are supposed to burn grass to find hidden rare candies underneath.
    - You can switch between the player and the pokemon by pressing B.
    - You can shoot fireballs *ONLY* as the pokemon by pressing A.
    - The fireballs burn the flowers.
    - Collect 3 rare candies to evolve into Charmeleon and be able to advance to Petalburg.
    - You have 5 fireballs. Use them wisely. If you run out before finding all of the rare candy, you lose.
    - The candies spawn in the same spot every time you respawn, so this level is ultimately a memorization game. 
    - ***The fireballs need to be placed in the middle of the flowers. This is also a game of precision.***
    - I have everything done except for tile modification and some burning animations.
    - You have 8 fireballs for level 2.