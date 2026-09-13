# About
Atom3D is a small 3D game engine written in C++ and OpenGL 3.3

## Features & Mechanics
Atom3D features:

* Entity Component System (OOP based)
* Memory Pool
* .obj files loading
* Custom scene file loading
* Bullet Physics
* Phong lighting
  * Directional Light
  * Point Light
  * Spot Light
* ImGui Debug UI
* Skybox
* SDL audio
### Custom .sc file
Atom3D scene loading has a custom .sc file.
The general structure looks like this:

* newobj Person 
* p 0.0 0.0 0.0  
* r 0.0 0.0 0.0  
* s 0.5 0.5 0.5
* mesh Assets/Object/model.obj 
* p_mass 50      
* new_comp charController 

Commands:
* newobj - creates a new object with a name
* p - sets the position of object
* r - sets the rotation of object
* s - sets scale of object
* mesh - mesh file of object
* p_mass - mass of object, 0 for static objects
* new_comp - adds components

### Screenshots and images
<img width="2560" height="1440" alt="image" src="https://github.com/user-attachments/assets/bbb2475f-36f8-415d-a64f-70d99738d538" />

* ImGui Debug UI with actors list and FPS
* model loading with physics
* Skybox found in _"learnopengl.com"_
* Container textures made by me
### Basic controls
* WASD to move camera
* ESC to free mouse

## Build
- Clone the repository
- Configure in .lua files 
- Run the premake scripts in Scripts/ . With your systems operating system
- Open the generated visual studio project
## System requirements
* Visual studio 2017
* OpenGL 3.3
## Credits
- Thanks Joey de Vries (creator of _"learnopengl.com"_) for parts of code and tutorials
- Thanks Yan Chernikov (known as _The Cherno_) for the project template

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
