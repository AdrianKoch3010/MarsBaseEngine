#pragma once

/// @file
/// @brief Class mbe::TextureComponent

#include <cassert>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Graphics/TextureWrapper.h>

namespace mbe
{

	class TextureWrapperComponent : public Component
	{
	public:
		typedef std::shared_ptr<TextureWrapperComponent> Ptr;
		typedef std::weak_ptr<TextureWrapperComponent> WPtr;
		typedef std::unique_ptr<TextureWrapperComponent> UPtr;

	public:
		TextureWrapperComponent(Entity & parentEntity, const TextureWrapper & textureWrapper);
		~TextureWrapperComponent() = default;

	public:
		void SetTextureWrapper(const TextureWrapper & textureWrapper, bool resetTextureRect = true);

		void SetTextureRect(const sf::IntRect & textureRect);

		void SetTextureRect(sf::IntRect && textureRect);

		inline const TextureWrapper & GetTextureWrapper() const { return *textureWrapper; }

		inline const sf::IntRect & GetTextureRect() const { return textureRect; }

	private:
		const TextureWrapper * textureWrapper;
		sf::IntRect textureRect;
	};

} // namespace mbe