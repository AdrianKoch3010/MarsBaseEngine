# Documentation
The engine is documented using doxygen.
The tutorial below provides some worked examples of how to use the Mars Base Engine.

?> The full documentation can be found [here](https://adriankoch3010.github.io/MarsBaseEngine/Doxygen/html/index.html).

# Demo
I am using the engine to develop a game called 'The Mars Base Project'. The Mars Base Project is a 2D city building game set on Mars that puts its focus on building a sustainable base while caring about the needs of the astronauts living there. It heavily relies on the simulation of the individual inhabitants in the likes of games such as Banished or Planet Base.

Its development is documented in a series of devlog videos which can be found on my YouTube channel [Domex Devlogs](https://www.youtube.com/channel/UC8SGfIwAkb97874nAoMN3rg?view_as=subscriber).

<div style="text-align:center; width:100%;">
<iframe width="795" height="447" src="https://www.youtube.com/embed/NfRin9Q3NWU" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>

# Tutorials
A few quick words on the structure of the following tutorials. The aim is to give an easy to understand reference point for users starting to learn the engine. It is further intended to give an overview of the functionality provided together with a short explanation and pieces of code showing how to use them in principle. This is by no means exhaustive; an (almost) complete documentation of the API can be found [here](Doxygen/index.html). Note, however, that this does not include explanation and should, therefore, be used as a reference only.

The engine is divided into the following modules. The tutorials follow this structure by providing an overview of the core fucntionality of the differnt modules, focussing on their most imporant uses.

<!-- - Download and Compilation
- First steps -->
<!-- - [Sound and Music](Tutorials/Audio.md) -->

# Modules

- [AI](Tutorials/AI/AI.md)
- [Animation](Tutorials/Animation/Animation.md)
- [Core](Tutorials/Core/Core.md)
    - [The entity component system](Tutorials/Core/EntityComponentSystem.md)
    - [Game loop and state system](Tutorials/Core/StateSystem.md)
    - [Working with events](Tutorials/Core/Events.md)
    - [HandleIDs](Tutorials/Core/HandleID.md)
    - [Resource management](Tutorials/Core/ResourceManagement.md)
- [Graphics](Tutorials/Graphics/Graphics.md)
- [Input](Tutorials/Input/Input.md)
- [Serialisation](Tutorials/Serialisation/Serialisation.md)