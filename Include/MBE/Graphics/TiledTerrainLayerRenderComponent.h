#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerRenderComponent

#include <iostream>
#include <cassert>

#include <SFML/Graphics.hpp>

#include <MBE/Graphics/RenderComponent.h>
#include <MBE/TransformComponent.h>
#include <MBE/TiledTerrainLayerComponent.h>

namespace mbe
{

	class TiledTerrainLayerRenderComponent : public RenderComponent
	{
		friend class TiledTerrainLayerSystem;
	public:
		TiledTerrainLayerRenderComponent(Entity & parentEntity, const sf::Texture & tilesetTexture);
		~TiledTerrainLayerRenderComponent() = default;

	public:
		void Draw(sf::RenderTarget & target) override;

		sf::FloatRect GetBoundingBox() const override;

		inline bool IsCreated() const { return isCreated; }

		inline sf::VertexArray & GetVertices() { return vertices; }

		inline const sf::Texture & GetTexture() const { return tilesetTexture; }

	private:
		inline void SetCreated(bool value = true) { isCreated = value; }

	private:
		sf::VertexArray vertices;
		const sf::Texture & tilesetTexture;

		bool isCreated;
	};

} // namespace mbe
