#pragma once

#include <cassert>
#include <SFML/Graphics/Sprite.hpp>

#include <MBE/Graphics/TexturedRenderComponent.h>
#include <MBE/TransformComponent.h>
#include <MBE/Core/HandleBase.h>

namespace mbe
{

	class SpriteRenderComponent : public TexturedRenderComponent, public HandleBase<SpriteRenderComponent>
	{
	public:
		// Add constructor overloads
		SpriteRenderComponent(Entity & parentEntity, const TextureWrapper & textureWrapper, RenderLayer renderLayer = RenderLayer::GUI);
		~SpriteRenderComponent() = default;

	public:
		void Draw(sf::RenderTarget & target) override;

		sf::FloatRect GetBoundingBox() const override;

		inline void SetColor(const sf::Color & color) { sprite.setColor(color); }

		// For the frame animation - must be changed
		inline void SetOrigin(const sf::Vector2f & origin) { parentEntity.GetComponent<TransformComponent>().SetOrigin(origin); }

		inline const sf::Color & GetColor() const { return sprite.getColor(); }

	private:
		sf::Sprite sprite;
	};
} // namespace mbe