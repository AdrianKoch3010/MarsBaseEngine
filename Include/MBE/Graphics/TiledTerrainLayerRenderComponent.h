#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerRenderComponent

#include <iostream>
#include <cassert>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include <MBE/Graphics/RenderComponent.h>
#include <MBE/TransformComponent.h>


namespace mbe
{
	// Does not store information on the tile index list
	// Some other system must take care of syncronysing it with the tiled terrain data
	class TiledTerrainLayerRenderComponent : public RenderComponent
	{
	public:
		TiledTerrainLayerRenderComponent(Entity & parentEntity, const sf::Texture & tilesetTexture, sf::Vector2u size, sf::Vector2u tileSize);
		~TiledTerrainLayerRenderComponent() = default;

	public:
		void Draw(sf::RenderTarget & target) const override;

		sf::FloatRect GetBoundingBox() const override;

		void Create(std::vector<size_t> tileIndexList);

		void SetTile(sf::Vector2u position, size_t tileIndex);

		inline bool IsCreated() const { return isCreated; }

		inline sf::VertexArray & GetVertices() { return vertices; }

		inline const sf::Texture & GetTexture() const { return tilesetTexture; }

		// Throws
		//size_t GetTile(sf::Vector2u position) const;

		inline const sf::Vector2u & GetSize() const { return size; }
		inline const sf::Vector2u & GetTileSize() const { return tileSize; }

	public:
		/// @brief A constant defining an empty tile. This is a transparrent tile that has no texture associated with it.
		static const int emptyTile = -1;

	private:
		sf::VertexArray vertices;
		const sf::Texture & tilesetTexture;

		bool isCreated;
		const sf::Vector2u size;
		const sf::Vector2u tileSize;
	};

} // namespace mbe
