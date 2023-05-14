# Moteur

Submission for Game Jam / Tech Demo - 2023.

## Build Instructions
- Built using Visual Studio 17 (2022) - Retarget solution if necessary.
- All build requirements and other files are included within the project.

## Controls
- F key : control the player character.
- W,A,S,D keys : Move the character.
- Z key : Toggle sprint.
- SPACE : Jump.
- C key : Control the camera.
- F1 key : Toggle pan camera with the mouse.
- LEFT ALT key : Press to toggle cursor. (to Maximize the window/Interact with GUI)
- Most other controls can be found on the GUI drawn with ImGUI.

## Physics Information
- The physics in this game engine is provided by a third party library called bullet physics.
- A gravitational force pulls all the objects towards the ground.
- The scene objects and the ground all have collisions on them.
- The player character utilizes the Character Controller class in the bullet physics library to implement character movement.

## AI Information
- All the AIs in the game follow a state system and decision tables to determine their next action.
- The AIs can have 3 states which are are "IdleState", "PursueState", and "CatchState".
- "IdleState" : AI does nothing and basically just stands there.
- "PursueState" : AI has discovered the player and actively pursues them.
- "CatchState" : AI has caught up to the player and has rendered them immobile.
- In "CatchState" both the player and the AI will attack each other and their displayed health on the HUD will go down.
- The "CatchState" gives the player a certain time window to escape every time the player is caught by them. The player can run away or continue attacking the AI, eventually killing or getting killed by the AI.

## Animation Information
- All the character models in the game have their own skin mesh animations assigned to them.
- These animations generated using Adobe's tool Mixamo, and are loaded with the help of Assimp.
- Similar to the AIs, these animations are switched automatically depending on the actions/states of the AI or the player.
- Every character model has at least 2 animations assigned to them.
- Animation Blending: Switching animations smoothly blends the transition between the previous and the next states.

## UML diagram
The diagram is in the root directory of this project. Folder name is `uml_diagram`

## Video Demo
A video demonstration of the project is included within `video-demo` folder in solution directory.

## Git Repository
- The git repo for this project is [Moteur](https://github.com/M3rl3/Moteur).
- Look at branch name `staging`.

## Game Engine Details
- This game engine was designed using the ECS (Entity, Component, System) Pattern.
- A thrid party physics engine (Bullet Physics) is used to implement physics into this engine via Components.
- Models and animations are loaded with the help of Open Asset Import Library (assimp) (ver. 3.2.0).
- Sounds and music are loaded/played with the help of FMOD.
- Textures (PNG & BMP) are loaded using a library called stb_image.

## Gameplay Information
- The main goal of the player is to invade a camp belonging a group of evil Mercenaries.
- Player must kill all of the enemies to win the game.
- If the player stands close to an enemy, they automatically deal damage to them.
- The enemy will try to subdue the player and chase them but the player must escape the enemy camp alive.
- An enemy that got killed simply disappears.