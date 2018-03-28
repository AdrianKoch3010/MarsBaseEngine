//#include <MBE/TiledTerrainLayer.h>
//
//using mbe::TiledTerrainLayer;
//
//
//TiledTerrainLayer::TiledTerrainLayer(const sf::Texture & tilesetTexture, const sf::Vector2u size, const sf::Vector2u tileSize) :
//	tilesetTexture(tilesetTexture),
//	size(size),
//	tileSize(tileSize),
//	isCreated(false)
//{
//}
//
//#include <iostream>
//#include <SFML/System/Clock.hpp>
//
//void TiledTerrainLayer::Create(const std::vector<size_t> tileList)
//{
//	sf::Clock clock;
//	clock.restart();
//
//	this->isCreated = true;
//
//	// Check whether the tileList has enough items for the size of the tile map layer
//	assert(tileList.size() == size.x * size.y && "There it not enough data to create this tile map layer");
//
//	// resize the vertex array to fit the tileMapSize
//	vertices.setPrimitiveType(sf::Quads);
//	vertices.resize(size.x * size.y * 4); // The 4 because of the quads
//
//										  // Populate the vertex array with one quad per tile
//	for (size_t i = 0; i < size.x; i++)
//	{
//		for (size_t j = 0; j < size.y; j++)
//		{
//			// Get the current tile number
//			int tileIndex = tileList[i + j * size.x];
//
//			// Set the tile at position i, j using the tileIndex
//			this->SetTile(tileIndex, sf::Vector2u(i, j));
//		}
//	}
//
//	// The passed in tile list is no longer needed, move it to the tileIndexList
//	tileIndexList = std::move(tileList);
//
//	std::cout << std::endl << "The tile map layer creation took: " << clock.getElapsedTime().asMicroseconds() << " microseconds";
//}
//
//void TiledTerrainLayer::SetTile(const size_t tileIndex, const sf::Vector2u pos)
//{
//	// Find the tiles position in the tileset texture
//	sf::Vector2u texPos;
//	texPos.x = tileIndex % (tilesetTexture.getSize().x / tileSize.x);
//	texPos.y = tileIndex / (tilesetTexture.getSize().x / tileSize.x); // integer division
//
//																	  // Get a pointer to the current tile's quad
//	sf::Vertex * quad = &vertices[(pos.x + pos.y * size.x) * 4]; // The 4 because of the quads
//
//																 // If the tile is empty (nothing should be drawn on this tile)
//	if (tileIndex == emptyTile)
//	{
//		for (size_t i = 0; i < 4; i++)
//			quad[i].position = sf::Vector2f(0.f, 0.f);
//		return;
//	}
//
//	// Performs pointer arithmetic by increasing the pointer through the index operator
//	// Define its 4 corners
//	quad[0].position = sf::Vector2f(pos.x * tileSize.x, pos.y * tileSize.y);
//	quad[1].position = sf::Vector2f((pos.x + 1) * tileSize.x, pos.y * tileSize.y);
//	quad[2].position = sf::Vector2f((pos.x + 1) * tileSize.x, (pos.y + 1) * tileSize.y);
//	quad[3].position = sf::Vector2f(pos.x * tileSize.x, (pos.y + 1) * tileSize.y);
//
//	// Define its 4 texture coordinates
//	quad[0].texCoords = sf::Vector2f(texPos.x * tileSize.x, texPos.y * tileSize.y);
//	quad[1].texCoords = sf::Vector2f((texPos.x + 1) * tileSize.x, texPos.y * tileSize.y);
//	quad[2].texCoords = sf::Vector2f((texPos.x + 1) * tileSize.x, (texPos.y + 1) * tileSize.y);
//	quad[3].texCoords = sf::Vector2f(texPos.x * tileSize.x, (texPos.y + 1) * tileSize.y);
//}
//
//size_t TiledTerrainLayer::GetTile(sf::Vector2u position)
//{
//	assert(position.x < this->size.x && position.y < this->size.y && "The map is smaller than the requested tile");
//	// This should never be the case
//	assert(position.x * position.y < tileIndexList.size() && "There are less tiles registered than required by the map size");
//	return tileIndexList[position.y * size.x + position.x];
//}
//
//void TiledTerrainLayer::draw(sf::RenderTarget & target, sf::RenderStates states) const
//{
//	// Only draw a map that has been created
//	if (!isCreated)
//		return;
//
//	// Apply the transform;
//	states.transform *= getTransform();
//
//	// Apply the tileset texture
//	states.texture = &tilesetTexture;
//
//	// Draw the vertex array
//	target.draw(vertices, states);
//}