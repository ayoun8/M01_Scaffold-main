***What my game has so far***
1. FULLS SPRITESHEET
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
    d. IN LEVEL 4;
        - LEFT: changes move to flamethrower
        - RIGHT: changes move to scratch
        - A: attack
    DEBUGGING
    d. Press RSHOULDER to go to level 2
    e. Press LSHOULDER to go to level 3
5. FUNCTIONING STATES:
    a. *Start, instructions, game, pause, win, lose, battle* states are all implemented
        - ***Winning and losing conditions are also implemented***
            - To win, beat onyx
            - To lose, lose to onyx, respawn to level 3
        - All states are functional.
6. FUNCTIONING TILEMAP AND TILESET FOR ALL FOUR LEVELS

***What needs to be added for my next milestone***
1. I need to lock down my rare candy initialization
    a. I need to initialize the candies as inactive (they're not truly hidden for now for easy debugging and level traversing)
2. Art:
    a. I need to add dialogue boxes
    b. I need to add instructions
    c. I need to add sounds and music
    d. I need to add a parallax background
    e. I need to add cut scenes.
3. Finish the fireball counter and recharge

***Bugs***
1. Rare candy spawn positions aren't as accurate as I want them to be.
2. The pause pauses things well but also regenerates the burnt tiles (but still keeps your progress).

***NAVIGATION***
a. Press START to start
b. Press SELECT in the start screen to see the ***instructions***
c. Press START once in game to pause
    - Press SELECT in the pause menu to go back to start
DEBUGGING
    d. Press RSHOULDER to go to level 2
    e. Press LSHOULDER to go to level 3

***Notes***
- My heal works and loads.
- I also did not hide the rare candies and just placed them in the center so they are easy to collect to test my game.
- You're more likely to lose when you use scratch
- You can access the exits once you're the next evolution. 
- The fireball delay timer could cause you to die if you advance too quickly (because you aren't supposed to normally).

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