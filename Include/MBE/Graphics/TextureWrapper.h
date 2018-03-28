#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <MBE/Core/PixelMask.h>

namespace mbe
{
	class TextureWrapper
	{
	public:
		TextureWrapper() = default;
		~TextureWrapper() = default;

	public:
		// To be used by the mbe::AssetHolder
		bool loadFromFile(std::string filePath, bool createPixelMask = false);

		// returns nullptr if the pixel mask has not been created yet
		inline const PixelMask::Ptr GetPixelMask() const { return pixelMask; }

		// returns nullptr if the texture has not been created yet
		//inline const std::shared_ptr<sf::Texture> GetTexture() const { return texture; }

		inline sf::Texture & GetTexture() { return *texture; }

		// const overload
		inline const sf::Texture & GetTexture() const { return *texture; }

	private:
		std::shared_ptr<sf::Texture> texture;
		PixelMask::Ptr pixelMask;
	};
}