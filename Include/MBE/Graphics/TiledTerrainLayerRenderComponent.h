#pragma once

/// @file
/// @brief Class mbe::TiledTerrainLayerRenderComponent

#include <iostream>
#include <cassert>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include <MBE/Graphics/RenderComponent.h>


namespace mbe
{
	// Does not store information on the tile index list
	// Some other system must take care of syncronysing it with the tiled terrain data
	class TiledTerrainLayerRenderComponent : public RenderComponent
	{
	public:
		TiledTerrainLayerRenderComponent(EventManager & eventManager, Entity & parentEntity, sf::Vector2u size, sf::Vector2u tileSize);
		~TiledTerrainLayerRenderComponent() = default;

	public:
		void Draw(sf::RenderTarget & target) const override;

		sf::FloatRect GetLocalBounds() const override;

	public:
		void Create(std::vector<size_t> tileIndexList);

		// Throws if no texture is set
		// Cannot calculate the texture coordinates for the tile
		void SetTile(sf::Vector2u position, size_t tileIndex);

		inline void SetRenderStates(const sf::RenderStates & states) { renderStates = states; }

		inline void SetRenderStates(sf::RenderStates && states) { renderStates = std::move(states); }

		inline bool IsCreated() const { return isCreated; }

		inline sf::VertexArray & GetVertices() { return vertices; }

		inline const sf::Vector2u & GetSize() const { return size; }

		inline const sf::Vector2u & GetTileSize() const { return tileSize; }

		inline const sf::RenderStates & GetRenderStates() const{ return renderStates; }

	public:
		/// @brief A constant defining an empty tile. This is a transparrent tile that has no texture associated with it.
		static const int emptyTile = -1;

	private:
		sf::VertexArray vertices;
		sf::RenderStates renderStates;

		bool isCreated;
		const sf::Vector2u size;
		const sf::Vector2u tileSize;
	};

} // namespace mbe
