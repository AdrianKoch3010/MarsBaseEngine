#include <MBE/Graphics/TiledTerrainLayerRenderComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(TiledTerrainLayerRenderComponent, RenderComponent)

TiledTerrainLayerRenderComponent::TiledTerrainLayerRenderComponent(Entity & parentEntity, sf::Vector2u size, sf::Vector2u tileSize) :
	RenderComponent(parentEntity),
	size(size),
	tileSize(tileSize),
	isCreated(false)
{
	assert(parentEntity.HasComponent<TransformComponent>() && "TiledTerrainLayerRenderComponent: The parent entity must have a mbe::TransformComponent");
	assert(parentEntity.HasComponent<TextureWrapperComponent>() && "TiledTerrainLayerRenderComponent: The parent entity must have a mbe::TextureWrapperComponent");
}

void TiledTerrainLayerRenderComponent::Draw(sf::RenderTarget & target) const
{
	// Only draw the vertices if the tiled terrain layer has been created
	if (this->IsCreated() == false)
		return;

	sf::RenderStates states;
	states.texture = &parentEntity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();
	states.transform = parentEntity.GetComponent<TransformComponent>().GetWorldTransform();

	target.draw(vertices, states);
}

sf::FloatRect TiledTerrainLayerRenderComponent::GetLocalBounds() const
{
	sf::FloatRect rect;

	rect.width = static_cast<float>(size.x * tileSize.x);
	rect.height = static_cast<float>(size.y * tileSize.y);

	return rect;
}

void TiledTerrainLayerRenderComponent::Create(std::vector<size_t> tileIndexList)
{
	sf::Clock clock;
	clock.restart();

	// Check whether the tileList has enough items for the size of the tile map layer
	assert(tileIndexList.size() == size.x * size.y && "The length of the tile list does not match the number of tiles required to create this layer");

	// resize the vertex array to fit the tileMapSize
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(size.x * size.y * 4); // The 4 because of the quads

										  // Populate the vertex array with one quad per tile
	for (size_t i = 0; i < size.x; i++)
	{
		for (size_t j = 0; j < size.y; j++)
		{
			// Get the current tile number
			int tileIndex = tileIndexList[i + j * size.x];

			// Set the tile at position i, j using the tileIndex
			this->SetTile({ i, j }, tileIndex);
		}
	}

	isCreated = true;

	std::cout << std::endl << "The tile map layer creation took: " << clock.getElapsedTime().asMicroseconds() << " microseconds";
}

void TiledTerrainLayerRenderComponent::SetTile(sf::Vector2u pos, size_t tileIndex)
{
	// Find the tiles position in the tileset texture
	sf::Vector2u texPos;
	const auto & tilesetTexture = parentEntity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();
	texPos.x = tileIndex % (tilesetTexture.getSize().x / tileSize.x);
	texPos.y = tileIndex / (tilesetTexture.getSize().x / tileSize.x); // integer division

	// Get a pointer to the current tile's quad
	sf::Vertex * quad = &vertices[(pos.x + pos.y * size.x) * 4]; // The 4 because of the quads

	// If the tile is empty (nothing should be drawn on this tile)
	if (tileIndex == emptyTile)
	{
		for (size_t i = 0; i < 4; i++)
			quad[i].position = { 0.f, 0.f };
		return;
	}

	// Performs pointer arithmetic by increasing the pointer through the index operator
	// Define its 4 corners
	quad[0].position = static_cast<sf::Vector2f>(sf::Vector2u(pos.x * tileSize.x, pos.y * tileSize.y));
	quad[1].position = static_cast<sf::Vector2f>(sf::Vector2u((pos.x + 1u) * tileSize.x, pos.y * tileSize.y));
	quad[2].position = static_cast<sf::Vector2f>(sf::Vector2u((pos.x + 1u) * tileSize.x, (pos.y + 1u) * tileSize.y));
	quad[3].position = static_cast<sf::Vector2f>(sf::Vector2u(pos.x * tileSize.x, (pos.y + 1u) * tileSize.y));

	// Define its 4 texture coordinates
	quad[0].texCoords = static_cast<sf::Vector2f>(sf::Vector2u(texPos.x * tileSize.x, texPos.y * tileSize.y));
	quad[1].texCoords = static_cast<sf::Vector2f>(sf::Vector2u((texPos.x + 1u) * tileSize.x, texPos.y * tileSize.y));
	quad[2].texCoords = static_cast<sf::Vector2f>(sf::Vector2u((texPos.x + 1u) * tileSize.x, (texPos.y + 1u) * tileSize.y));
	quad[3].texCoords = static_cast<sf::Vector2f>(sf::Vector2u(texPos.x * tileSize.x, (texPos.y + 1u) * tileSize.y));
}

//size_t TiledTerrainLayerRenderComponent::GetTile(sf::Vector2u position) const
//{
//	// This should never be the case
//	assert(position.x * position.y < tileIndexList.size() && "TiledTerrainLayerComponent: There are less tiles registered than required by the map size");
//
//	if (position.x >= this->size.x || position.y >= this->size.y)
//		throw std::out_of_range("TiledTerrainLayerComponent: The map is smaller than the requested tile");
//
//	return tileIndexList[position.y * size.x + position.x];
//}