#pragma once

/// @file
/// @brief Class mbe::TextureWrapper

#include <SFML/Graphics/Texture.hpp>

#include <MBE/Core/PixelMask.h>

namespace mbe
{

	class TextureWrapper
	{
	public:
		/// @brief Default constructor
		TextureWrapper() = default;

		/// @brief Default destructor
		~TextureWrapper() = default;

	public:
		/// @brief Loads a texture from a file and optionally creates a pixel mask for it
		/// @details The creating of a pixel mask can be fairly computationally expensive. Hence, this functions allows for
		/// calculating it at the same time as loading the textures (in the program flow, not actually in parallel).
		/// @param filePath The file path of the texture to be loaded
		/// @param createPixelMask If true, a pixel mask is created after loading the texture. If false, no further actions are taken
		/// @note This method has the same signature as the methods provided by SFML for loading assets from a file
		/// such as sf::Texture::loadFromFile(), sf::SoundBuffer::loadFromFile(). This enables the texture wrapper to be treated just
		/// like any other asset. Therefore, texture wrappers can be stored in an mbe::TextureWrapperHolder
		/// @see mbe::AssetHolder
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