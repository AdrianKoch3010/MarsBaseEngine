#include <MBE/Graphics/TextureWrapper.h>

using mbe::TextureWrapper;

bool TextureWrapper::loadFromFile(std::string filePath, bool createPixelMask)
{
	texture = std::make_shared<sf::Texture>();
	if (!texture->loadFromFile(filePath))
		return false;


	if (createPixelMask)
		pixelMask = std::make_shared<PixelMask>(*texture);

	return true;
}