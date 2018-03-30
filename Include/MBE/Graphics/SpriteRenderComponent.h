#pragma once

#include <cassert>
#include <SFML/Graphics/Sprite.hpp>

#include <MBE/Graphics/TexturedRenderComponent.h>
#include <MBE/TransformComponent.h>
#include <MBE/Core/HandleBase.h>

namespace mbe
{

	class SpriteRenderComponent : public TexturedRenderComponent/*, public HandleBase<SpriteRenderComponent>*/
	{
	public:
		// Add constructor overloads
		SpriteRenderComponent(Entity & parentEntity, const TextureWrapper & textureWrapper, RenderObject::RenderLayer renderLayer = RenderObject::RenderLayer::GUI);
		~SpriteRenderComponent() = default;

	public:
		void Draw(sf::RenderTarget & target) const override;

		sf::FloatRect GetBoundingBox() const override;

		void SetColor(const sf::Color & color) { sprite.setColor(color); }

		// For the frame animation - must be changed
		inline void SetOrigin(const sf::Vector2f & origin) { parentEntity.GetComponent<TransformComponent>().SetOrigin(origin); }

		inline const sf::Color & GetColor() const { return sprite.getColor(); }

	private:
		mutable sf::Sprite sprite;
	};
} // namespace mbe