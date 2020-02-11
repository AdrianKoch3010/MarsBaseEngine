#pragma once

/// @file
/// @brief Class mbe::SpriteRenderComponent

#include <cassert>
#include <SFML/Graphics/Sprite.hpp>

#include <MBE/Graphics/RenderComponent.h>

namespace mbe
{

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		// Add constructor overloads
		SpriteRenderComponent(EventManager& eventManager, Entity& parentEntity);
		~SpriteRenderComponent() = default;

	public:

		void Draw(sf::RenderTarget& target) const override;

		sf::FloatRect GetLocalBounds() const override;

		inline const sf::Color& GetColor() const { return sprite.getColor(); }

		inline const sf::Texture* GetTexture() const { return sprite.getTexture(); }

		inline const sf::IntRect& GetTextureRect() const { return sprite.getTextureRect(); }

		inline const sf::Transform& GetTransform() const { return transform; }

		inline void SetColor(const sf::Color& color) { sprite.setColor(color); }

		inline void SetTexture(const sf::Texture& texture, bool resetRect = false) { sprite.setTexture(texture, resetRect); }

		inline void SetTextureRect(const sf::IntRect& textureRect) { sprite.setTextureRect(textureRect); }

		inline void SetTransform(const sf::Transform& transform) { this->transform = transform; }

	private:
		mutable sf::Sprite sprite;

		sf::Transform transform;
	};

} // namespace mbe