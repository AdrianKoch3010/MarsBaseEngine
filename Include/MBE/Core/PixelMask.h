#pragma once

/// @file
/// @brief Class mbe::detail::PixelMask

#include <iostream> // for error messages and time management
#include <vector>
#include <memory>
#include <algorithm>

#include <SFML\Graphics.hpp>

namespace mbe
{

	class PixelMask
	{
	public:
		typedef std::shared_ptr<PixelMask> Ptr;

	public:
		PixelMask(const sf::Texture & texture);
		// The sprite must have a texture - otherwise undefined behaviour
		PixelMask(const sf::Sprite & sprite);
		PixelMask(const sf::RectangleShape & rectangle);
		//~PixelMask() = default;
		~PixelMask() { std::cerr << "\nPixelMask Destructor" << std::endl; }

	public:
		bool Contains(sf::Vector2f point) const;
		inline bool Contains(float x, float y) const { this->Contains({ x, y }); }
		bool Contains(sf::Vector2f point, const sf::IntRect & subRect) const;

	private:
		void CreateMaskFromTexture(const sf::Texture &);

	private:
		const sf::Vector2u pixelMaskSize; // Same as texture size
		std::unique_ptr<bool[]> pixelMask;
	};

} // namespace mbe