# Moteur
The core engine for Ogun's Dice

### Requirements for Image compression

1. `pngquant`

    a. Download the latest version of [pngquant](https://pngquant.org/)

    b. Extract it to any location

    c. Set `\pngquant\pngquant.exe` to executable in your environment variable


2. `optipng`
    a. Download the latest version of [optipng](https://sourceforge.net/projects/optipng/)

    b. Extract it to any location

    c. Set `\optipng-0.7.7-win32\optipng-0.7.7-win32\optipng.exe` to executable in your environment variable


3. `PVRTexTool`
    a. Download the latest version of [PVRTexTool](https://developer.imaginationtech.com/downloads/), download `PVRTexTool`, it requires to login before you can download

    b. Run the installer, after completed, copy this path `C:\Imagination Technologies\PowerVR_Graphics\PowerVR_Tools\PVRTexTool\CLI\Windows_x86_641` to your environment variable

### Python script location
The python script is located in `..\assets\scripts\png_compressor.py`

### How to build

1. Open project in `visual studio 2022` and select build
2. Select `Debug` and `Start Debugging` to run.

#### Character movement
1. Press `f` to enable movement of the main character
2. User the `w`, `s`, `d` and `a` keys to move around

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
2. Ademola Adedeji
