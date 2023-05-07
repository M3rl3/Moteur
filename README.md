# Moteur

Submission for Game Jam / Tech Demo - 2023.

## Build Instructions:
- Built using Visual Studio 17 (2022) - Retarget solution if necessary.
- All build requirements and other files are included within the project.

## Controls
- W,A,S,D keys : Move the character.
- C key : control the camera.
- F1 key : Enable/Disable pan camera with the mouse.
- LEFT ALT key : Press and Hold to show cursor. (to Maximize the window/Interact with GUI)
- Most other controls can be found on the GUI drawn with ImGUI.

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
- Meshes are loaded with the help of Assimp library.
- Textures (PNG & BMP) are loaded using a library called stb_image.
- The play character is controlled using the Character Controller class from Bullet Physics.

## Gameplay Information
- The main goal of the player is to invade a camp belonging a group of evil Mercenaries.
- Player must kill all of the enemies to win the game.
- If the player stands close to an enemy, they automatically deal damage to them.
- The enemy will try to subdue the player and chase them but the player must escape the enemy camp alive.
- An enemy that got killed simply disappears.