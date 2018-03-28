#pragma once

/// @file
/// @brief Class mbe::TexturedRenderComponent

#include <memory>
#include <cassert>

#include <SFML/Graphics/Rect.hpp>

#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/TextureWrapper.h>

namespace mbe
{
	class TexturedRenderComponent : public RenderComponent
	{
	public:
		typedef std::shared_ptr<TexturedRenderComponent> Ptr;
		typedef std::weak_ptr<TexturedRenderComponent> WPtr;
		typedef std::unique_ptr<TexturedRenderComponent> UPtr;

	public:
		TexturedRenderComponent(Entity & parentEntity, const TextureWrapper & textureWrapper, RenderLayer renderLayer = RenderLayer::GUI);
		virtual ~TexturedRenderComponent() = default;

	public:
		// Resets the texture rect
		void SetTextureWrapper(const TextureWrapper & textureWrapper, bool resetTextureRect = true);
		void SetTextureRect(const sf::IntRect & textureRect);

		inline const TextureWrapper & GetTextureWrapper() const { return *textureWrapper; }
		inline const sf::IntRect & GetTextureRect() const { return textureRect; }

	private:
		// Use shared pointer
		// Const pointer instead of reference, so that the texture can be changed
		const TextureWrapper * textureWrapper;
		sf::IntRect textureRect;
	};

} // namespace mbe