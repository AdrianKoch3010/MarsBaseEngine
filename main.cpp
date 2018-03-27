// Documetation for cpplinq
// https://cpplinq.codeplex.com/wikipage?title=LINQ%20for%20C%2b%2b%20-%20getting%20started&referringTitle=Home

// Documentation for SFML
// https://www.sfml-dev.org/learn.php

// Video on how to create perfect collision using SFML
// https://www.youtube.com/watch?v=ffqvXKhrMHU

// How to get the possible screen resolutions using SFML
// https://en.sfml-dev.org/forums/index.php?topic=5804.0

// SFML Game Development example code
// https://github.com/SFML/SFML-Game-Development-Book

// Documentation for the TGUI GUI library
// https://tgui.eu/

// TinyXML2 tutorial
// https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/

// Picture describing this engine
// https://xkcd.com/844/

// Other games that might be interresting
// http://store.steampowered.com/app/294100/RimWorld/
// http://store.steampowered.com/app/328080/Rise_to_Ruins/

#include <iostream>
#include <string>
#include <conio.h>

#include "../Game/Application.h"

int main()
{
	Application app;
	app.Run();

	std::cout << std::endl << std::endl << std::endl << "Press any key to end the program...";
	_getch();
	return EXIT_SUCCESS;
}

/// Make the inputHandler case insensitive

/// Find a better way for managing views - not only render components can be part of a view
/// This may also apply to non renderable components
/// - make a render layer / view component ?

/// The Clickable System can determine which entity lies higher in the order of RenderLayer + zOrder
/// The Clickable component could have a boolean determining whether clicks can pass through it or are absorbed by it
/// Also the EntityClickedEvent could be extended to support a value representing the 'depth' of click ie how many clickable entities lie above it
/// Problem: not all clickable entities should require a render component

/// Maybe make a seperate texture component from which the sprite render component can access the texture
/// This makes it possible to have a textured shape for example

/// Implement culling in the draw methods of the render component and tileMapLayer
/// Also add some sort of sound culling
/// - For a start, this could eb directly tied to the render components culling

/// What happens if the texture rect is too big for the texture?

/// Add a way to interrupt a movement
/// Add checks to make sure the path is doable --> only one step at a time

/// Is an update method for components really needed? Or should they only contain methods for altering data?
/// - atm only the animation component and the clickable component need an update methods
/// - this could be implemented in the respective systems
/// - for now its not too big of an issue though

/// Document the group stuff - is it really needed? (It does not hurt)

////////////////////////////////////// Animation
/// Problem:
///	The animation component can't be fetched directly due to the extra template parameter
///
/// Have a seperate animation system and mbe::AnimateEntity event
/// Similar to how the MovementSystem maintains its CharacterMovers, the Animation System will maintain Animators that animate entities
/// In theory the need for templates is removed since entity.GetComponent<> can be used in the Animation
/// 
/// The question is whether the animation itself is stored in the animation component of the entity - it would make sense
///
/// If all animations only animate an enity, the need for the Animated template is also removed
///
/// Check whether the animation component is good the way it is
//////////////////////////////////////

////////////////////////////////////// Sound
/// How to make the sound position independent of resolution. i.e tie it to the screen aspect ratio:
/// - e.g. define the screen width as 1.f or 100.f and base all other values of that calculation. Otherwise the sound may be more distorted on larger screens
/// - This could also be solved by making the on screen position in general independent of screen resolution
/// --> It should not really be necessary when using the inverse transform of the view. If more is shown on screen, the attenuation could be changed to account for that
///
/// An alternative approach to the problem of having different ways to find the sound position (renderComponent + transformComponent + audioComponent) could be polymorphism.
/// Similar to how the spriteComponent implements the renderComponent's draw and getBoundingBox method there could be relativeAudioComponent and a visibleAudioComponent
/// implementing the GetPosition() method
///
/// Should the sound system call SoundManager::RemoveStoppedSounds()?
///
/// Implement some form of sound culling
//////////////////////////////////////

////////////////////////////////////// Map
/// Create a map class
/// Add the AStarPathfinder template requirements to the map class
/// Rename mbe::TileMap to mbe::TileMapRenderer since it is actually concirned with drawing the layers

/// Change the name of the TileMap to TiledTerrain
///
/// The tile map has an instance of the terrain class but also manages all the buildings and stuff
////////////////////////////////////// 

////////////////////////////////////// Highlight component
/// Implement the Highlight Component
/// Needs to change the texture / draw on its own
/// Maybe inherit from RenderComponent and add a highlightable option
/// Or add the option to add effects to the render container --> all the drawing is at one place
/// Maybe use a specific (static) FX Class for the Shaders or only an AssetHolder<std::string, sf::Shader>
/// It could provide a method to apply an effect to a drawable -> returns a new drawable with the applied effect
/// Check how performance is affected using the above method (should not be too much since the texture is a reference)
/// --> how to keep the texture alive for drawing (maybe use std::shared_ptr)
///
/// For the FXs:
/// An Fx is just a function of a specific signature
/// So maybe add this function as a content of an event
///
/// In order to keep it simple, this could also be done by either a plane old frame animation or a set texture animation
/// Also FX that can be set in a single shader would be preferred
/// Since the creation of a render texture is quite heavy, the render system might have its own render texture (maybe more than one to account for differnt sizes)
/// The FX would need to make use of these render textures to avoid having to create their own
//////////////////////////////////////

////////////////////////////////////// Constant File
/// Add the Textures and Fonts file paths to the mbe::Constants --> or make them inheritable so that there can be different constants singletons
/// The constants singleton should only manage the initialisation file containing things such as the resolution fade distance and so on.
/// All other information should be stored in either xml or sql
//////////////////////////////////////

////////////////////////////////////// Entity
/// Is the entity removed event still needed?
/// Maybe change this to an entity deleted event
/// Perhaps add two other events: EntityAdded and EntityRemoved
///
/// Should the parenting component be integrated into the entity class:
/// Every entity has a parent entity (which may be null) and a list of child entities which may be empty
//////////////////////////////////////