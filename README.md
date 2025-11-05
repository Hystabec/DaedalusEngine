<img align="center" alt="Daedalus banner" src="/resources/DD_Banner_V1.png"/>
<h1></h1>

Daedalus Engine is a personal game engine project that i'm developing using tutorials and guides to improve my C++ skills as well as to help improve my knowledge of game engine and architecting large software systems.  

Primarily following [The Cherno's - "Game Engine" series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) when developing the engine.  

Additional Libraries: [Premake](https://github.com/premake/premake-core), [GLFW](https://www.glfw.org), [GLEW](https://glew.sourceforge.net), [stb](https://github.com/nothings/stb), [spdlog](https://github.com/gabime/spdlog), [Dear ImGui](https://github.com/ocornut/imgui), [Entt](https://github.com/skypjack/entt), [yaml-cpp](https://github.com/jbeder/yaml-cpp), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo), SPIR-V ([Vulkan](https://www.vulkan.org)), [box2d](https://github.com/erincatto/box2d), [Mono](https://www.mono-project.com)  

## Current Features
* Custom maths library.  
* Event system.  
* Rendering with `OpenGL`.  
* 2D batch rendering.  
* Texture/Image rendering.  
* User Interface using `Dear ImGui`.  
* Custom Logging library primarily using `spdlog`.  
* Performance profiling using `chrome://tracing`.  
* Entity component system using `Entt`.  
* 2D physics using `Box2D`.  
* Shader cross compilation using `SPIR-V`.  
* Scripting support (C#) using `Mono`.  
* An Editor application (in development).  

## Planned Features
* Suppot for Mac and Linux.  
* Audio system.  
* Rendering with `Vulkan`.  
* Rendering with `DirectX`.  
* Rendering with `Metal`.  
* 3D batch rendering.  
* 3D physics engine.  
* A debugging extension for visual studio to debug scripts during runtime.  
* A complete editor application.  
* A standalone runtime environment.  
    * That can run projects built in the editor.  

## Making and Editing scripts
Currently the C# scripts are located in `DaedalusEngine/Daedalus-Editor/sandboxProject/assets/scripts`.  
Running `win-GenerateProject-VS22.bat` will generate as VS solution file that will open the C# solution.  
Note: The C# solution will need to be built before any of the default script can be ran (On a new clone of this repository).  

## Debugging Scritps
The visual studio debugger can be attached to a running instance of Daedalus to allow for the stepping of script and viewing of active variables.  
Currently to achive this `Unity`'s visual studio 2022 debugger can be used.  
To attach the unity debugger while using visual studio 2022: On the Top Menu click `Debug -> Attach Unity Debugger` then click `Input IP` and used the IP:`127.0.0.1:2550` then click OK.  
Eventually I plan to create my own extension for visual studio, but after unsuccessfuly trying to make one I have decided this will do for now.  

## Getting Started
<ins>1. Downloading the project files:</ins>

### Clone Using Git
clone using `git clone --recursive https://github.com/Hystabec/DaedalusEngine`.  
if not cloned recursively, use `git submodule update --init` to clone submodules.  

### Downloading From GitHub
Download all the project files.  
Download the additional submodules which can be found in `Daedalus-Core/Dependencies`.  
    <sub>Current submodules: [ImGuizmo](https://github.com/Hystabec/ImGuizmo/tree/2310acda820d7383d4c4884b7945ada92cd16a47), [yaml-cpp](https://github.com/Hystabec/yaml-cpp/tree/4ea8b0cc00ebe7cc2ce2bae18b84cf0f9767e8a2), [Box2D](https://github.com/Hystabec/box2d/tree/43e0742c4ac5777169ce4e5415236acdc176b1b6).</sub>  
All submodules can be viewed in `.gitmodules`. This file shows the `URLs` to download from and the `path` to go into.  

<ins>2. External Dependencies And Project Setup:</ins>
1. Run `setup.bat` located in the `setup scripts` folder. This will check for any required external dependencies and acquire them if they are not found.  
2. If the Vulkan SDK is not installed, the user will be prompted to install it. Make sure to install `Shader Toolchain Debug Symbols - 64-bit` when promted by the `vulkan` setup wizard, this will install the required debug libraries.  
3. Run `setup.bat` again to verify that the external dependencies are present.  
4. Run `win-GenerateProject-VS22.bat` to generate the visual studio solution.  

Visual studio solution can be regenerated at anytime by running `win-GenerateProject-VS22.bat`