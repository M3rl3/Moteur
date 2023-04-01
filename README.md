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

### Setup Boost and Thrift
1. Install Boost version `boost_1_59_0`, run the setup file and allow it to install in default directory
2. Create a folder in drive `c:`, name it `Thrift`
3. Download `Thrift 0.9.1`, extract the zip file to your drive `c:\Thrift`.

### How to run
1. There are two major project, `ServerApplication` and `ClientApplication`. `ServerApplication` runs the `thrift` server and `ClientApplication` has the game play.
2. Please run the `ServerApplication` first before `ClientApplication`

### Configure Boost
1. Include => All Configurations - All Platforms: "C:\local\boost_1_59_0"
2. Lib => All Configurations - x64: "C:\local\boost_1_59_0\lib64-msvc-14.0" (Note this is for `x64`)
3. Linker input for Debug mode => All Platforms: "libboost_thread-vc140-mt-gd-1_59.lib"
4. Linker input for Release mode => All Platforms: "libboost_thread-vc140-mt-1_59.lib"

### Configure Thrift
1. Include => All Configurations - All Platforms: "C:\Thrift\thrift-0.9.1\src"
2. Lib => All Configurations - All Platforms: "C:\Thrift\thrift-0.9.1\lib\$(IntDir)"
3. Linker input => All Configurations - All Platforms: "libthrift.lib"


### Python script location
The python script is located in `..\assets\scripts\png_compressor.py`

### How to build

1. Open project in `visual studio 2022` and select build
2. Select `Debug` and `Start Debugging` to run.

#### Character movement
1. Press `f` to enable movement of the main character
2. User the `w`, `s`, `d` and `a` keys to move around

#### Leaderboard
1. If you press `p` it will print the top 20 high scores in the console. We tried ImGui for this implementation but ran into so many issue.

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
