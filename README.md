# MarsBaseEngine
The Mars Base Engine (MBE) is a light-weight 2D game engine based on SFML.

## Features

- An event system
- An entity component system
- A state system for managing program states
- A 2D rendering engine with support for glsl shaders
- A tile map functionality for easy and performant tile rendering
- A audio system allowing for spatialized sounds and music
- An animation system that can be used animate all kinds of objects (not just graphics)

### How about GUI?
There is no dedicated GUI module. However, due to the SFML rendering pipeline, third party libraries such as [TGUI](https://tgui.eu/ "TGUI Homepage") can be used. Either directly by creating a new render component and overloading the draw method or by simply drawing directly to the SFML render window.

## Learn
A tutorial can be found [here](https://adriankoch3010.github.io/MarsBaseEngine/index.md "Mars Base Engine Tutorial").

The source code is documented using [doxigen](http://www.doxygen.nl/ "Doxygen Mainpage") and can be found in the docs folder. It also hosted [online](https://adriankoch3010.github.io/MarsBaseEngine/Doxygen/index.html "Mars Base Project Documentation").

## Libraries
The engine makes use of a number of open source libraries.

- [SFML](https://sfml-dev.org "SFML Homepage")
- [TinyXML2](https://github.com/leethomason/tinyxml2 "TinyXML2 GitHub page")

SFML 2.5.1 is required and must be linked with the project. A [tutorial](https://www.sfml-dev.org/tutorials/2.5/ "SFML Tutorial") showing how to set up SFML in different development environments can be found here. SFML itself is based on OpenGL and OpenAL to provide its graphics and audio functionality.

TinyXML2 is used as a source library and part of the Mars Base Engine"s source code.