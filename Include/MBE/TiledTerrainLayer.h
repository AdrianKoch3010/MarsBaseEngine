//#pragma once
//
///// @file
///// @brief Class mbe::TileMapLayer
//
//#include <array>
//#include <vector>
//#include <memory>
//#include <cassert>
//
//#include <SFML/Graphics.hpp>
//
//#include <MBE/Core/HandleBase.h>
//
//namespace mbe
//{
//	class TiledTerrainLayer : public sf::Drawable, public::sf::Transformable, private sf::NonCopyable, public HandleBase<TiledTerrainLayer>
//	{
//	public:
//		typedef std::unique_ptr<TiledTerrainLayer> UPtr;
//
//	public:
//		/// @brief Constructor
//		/// @param tilesetTexture The texture of the tileset
//		/// @param size The size of this map layer
//		/// @param tileSize The size of an individual tile
//		TiledTerrainLayer(const sf::Texture & tilesetTexture, const sf::Vector2u size, const sf::Vector2u tileSize);
//
//		/// @brief Default destructor
//		~TiledTerrainLayer() = default;
//
//	public:
//		// The tile index goes left to right top to bottom --> so basically row by row
//		void Create(const std::vector<size_t> tileList);
//
//		/// @brief Sets a single tile's index
//		/// @param tileIndex The index of the tile on the tile set texture. The tile index goes left to right, top to bottom
//		/// so basically row by row.
//		/// @param position The position in tiles on the tile map at which the tile shall be set
//		void SetTile(const size_t tileIndex, const sf::Vector2u position);
//
//		size_t GetTile(sf::Vector2u position);
//
//		inline bool IsCreated() const { return isCreated; }
//		inline const sf::Vector2u GetSize() const { return size; }
//		inline const sf::Vector2u GetTileSize() const { return tileSize; }
//		inline const std::vector<size_t> & GetTileIndexList() const { return tileIndexList; }
//
//		void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
//
//		/// @brief A constant defining an empty tile. This is a transparrent tile that has no texture associated with it.
//		static const int emptyTile = -1;
//
//	private:
//		sf::VertexArray vertices;
//		const sf::Texture & tilesetTexture;
//
//		bool isCreated;
//		const sf::Vector2u size;
//		const sf::Vector2u tileSize;
//
//		std::vector<size_t> tileIndexList;
//	};
//
//} // namespace mbe