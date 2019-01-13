#pragma once

#include <cassert>
#include <SFML/Graphics/Sprite.hpp>

#include <MBE/TransformComponent.h>
#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>

namespace mbe
{

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		// Add constructor overloads
		SpriteRenderComponent(Entity & parentEntity);
		~SpriteRenderComponent() = default;

	public:

		void Update(sf::Time deltaTime) override;

		void Draw(sf::RenderTarget & target) const override;

		sf::FloatRect GetLocalBounds() const override;

		inline void SetColor(const sf::Color & color) { sprite.setColor(color); }

		inline const sf::Color & GetColor() const { return sprite.getColor(); }

	private:
		mutable sf::Sprite sprite;
	};

} // namespace mbe