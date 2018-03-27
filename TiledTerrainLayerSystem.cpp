#include "TiledTerrainLayerSystem.h"

using namespace mbe;

TiledTerrainLayerSystem::TiledTerrainLayerSystem(EventManager & eventManager) :
	eventManager(eventManager)
{
	eventManager.Subscribe(EventManager::TCallback<event::EntityCreatedEvent>([this](const event::EntityCreatedEvent & event)
	{
		auto * entity = Entity::GetObjectFromID(event.GetEntityID());
		assert(entity != nullptr && "TiledTerrainLayerSystem: The entity must exists");
		OnEntityCreated(*entity);
	}));
}

void TiledTerrainLayerSystem::OnEntityCreated(Entity & entity)
{
	if (entity.HasComponent<TiledTerrainLayerComponent>() == false)
		return;

	if (entity.HasComponent<TiledTerrainLayerRenderComponent>() == false)
		return;

	auto & tiledTerrainLayerComponent = entity.GetComponent<TiledTerrainLayerComponent>();
	auto & tiledTerrainLayerRenderComponent = entity.GetComponent<TiledTerrainLayerRenderComponent>();

	sf::Clock clock;
	clock.restart();

	tiledTerrainLayerRenderComponent.SetCreated(true);

	const auto & tileList = tiledTerrainLayerComponent.GetTileIndexList();
	const auto & size = tiledTerrainLayerComponent.GetSize();
	auto & vertices = tiledTerrainLayerRenderComponent.GetVertices();

	// Check whether the tileList has enough items for the size of the tile map layer
	assert(tileList.size() == size.x * size.y && "The length of the tile list does not match the number of tiles required to create this layer");

	// resize the vertex array to fit the tileMapSize
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(size.x * size.y * 4); // The 4 because of the quads

										  // Populate the vertex array with one quad per tile
	for (size_t i = 0; i < size.x; i++)
	{
		for (size_t j = 0; j < size.y; j++)
		{
			// Get the current tile number
			int tileIndex = tileList[i + j * size.x];

			// Set the tile at position i, j using the tileIndex
			this->SetTile(entity, tileIndex, sf::Vector2u(i, j));
		}
	}

	std::cout << std::endl << "The tile map layer creation took: " << clock.getElapsedTime().asMicroseconds() << " microseconds";
}

void TiledTerrainLayerSystem::SetTile(Entity & entity, size_t tileIndex, sf::Vector2u pos)
{
	auto & tiledTerrainLayerComponent = entity.GetComponent<TiledTerrainLayerComponent>();
	auto & tiledTerrainLayerRenderComponent = entity.GetComponent<TiledTerrainLayerRenderComponent>();
	auto emptyTile = TiledTerrainLayerComponent::emptyTile;
	const auto & tileSize = tiledTerrainLayerComponent.GetTileSize();
	const auto & size = tiledTerrainLayerComponent.GetSize();
	const auto & tilesetTexture = tiledTerrainLayerRenderComponent.GetTexture();
	auto & vertices = tiledTerrainLayerRenderComponent.GetVertices();

	// Find the tiles position in the tileset texture
	sf::Vector2u texPos;
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
