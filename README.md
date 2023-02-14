# Moteur
The core engine for Ogun's Dice

### How to build

1. Open project in `visual studio 2022` and select build
2. Select `Debug` and `Start Debugging` to run.

#### Character movement

Left click a location on screen and the character will move to the location (`Position keyframe`)

#### UML diagram
The diagram is in the root directory of this project. Folder name is `uml_diagram`

#### Git Repository
The git repo for this project is [Moteur](https://github.com/M3rl3/Moteur) and we have granted access

#### External Libraries
1. `rapidjson` for loading meshes.

#### Design Patterns
1. FactoryMethod for creating character and object
2. Singleton for instanciating Object class

#### System Design
We created two engines and the second was designed using the ECS pattern while the first was not.
#### Contributors
1. Rameez Khatri
2. Jihye Yoon (Left the team)
3. Ademola Adedeji
