# MarsBaseEngine
The Mars Base Engine (MBE) is a light-weight 2D game engine based on SFML, OpenGL and OpenAL.

## Features

- An event system
- An entity component system
- A state system for managing program states
- A 2D rendering engine with support for shaders and effects
- A tile map functionality for easy and performant tile based rendering
- A sound system allowing for spatialized audio
- An animation system that can be used animate all kinds of objects (not just graphics)

### How about GUI?
There is no dedicated GUI module. However, due to the SFML rendering pipeline, third party libraries such as TGUI can be used. Either directly by creating a new render component and overloading the draw method or by simply drawing after the game by making direct use of the SFML render window.

## Libraries
The engine makes use of a number of open source libraries.

- SFML
- TinyXML2
