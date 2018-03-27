#include "State.h"
#include "StateManager.h"

using namespace mbe;

State::State(StateManager & stateManager, Context context) :
	stateManager(stateManager),
	context(context)
{
}

State::Context::Context(sf::RenderWindow & window, TextureWrapperHolder<std::string> & textures, SoundBufferHolder<std::string> & sounds, FontHolder<std::string> & fonts, FilePathDictionary<std::string> & musicFilePaths) :
	window(window),
	textureWrappers(textures),
	sounds(sounds),
	fonts(fonts),
	musicFilePaths(musicFilePaths)
{
}
