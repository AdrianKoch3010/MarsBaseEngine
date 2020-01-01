#include "..\..\..\Include\MBE\Map\TiledTerrain.h"
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/ComponentValueChangedEvent.h>

#include <MBE/Map/TiledTerrain.h>

using namespace mbe;
using mbe::event::EntityCreatedEvent;
using TextureChangedEvent = mbe::event::ComponentValueChangedEvent<mbe::TextureWrapperComponent>;
using IndexListChangedEvent = mbe::event::ComponentValueChangedEvent<mbe::TiledTerrainLayerComponent>;

// Remember to add subscriptions for the textureChangedEvent and indexListChangedEvent
TiledTerrain::TiledTerrain(EventManager& eventManager, EntityManager& entityManager, sf::Vector2u size, sf::Vector2u tileSize) :
	eventManager(eventManager),
	entityManager(entityManager),
	size(size),
	tileSize(tileSize)
{
	SubscribeEvents();
}

TiledTerrain::TiledTerrain(EventManager& eventManager, EntityManager& entityManager, Data::Ptr mapData, const std::string& tileMapTextureWrapperId) :
	eventManager(eventManager),
	entityManager(entityManager),
	size(mapData->GetMapSize()),
	tileSize(mapData->GetTileSize())
{
	SubscribeEvents();

	// Add layers
	for (const auto& layer : mapData->GetTileMapLayersIndexList())
	{
		Entity::HandleID layerId = this->AddTileMapLayer(tileMapTextureWrapperId);
		auto& layerEntity = *Entity::GetObjectFromID(layerId);

		try
		{
			// Must exist
			layerEntity.GetComponent<TiledTerrainLayerRenderComponent>().Create(layer);
		}
		catch (const std::runtime_error & error)
		{
			std::cerr << std::endl << error.what();
		}

		// Store the index list in the tiled terrain layer component
		// This must be called after creating the layer
		layerEntity.GetComponent<TiledTerrainLayerComponent>().SetIndexList(layer);

	}
}

TiledTerrain::~TiledTerrain()
{
	eventManager.UnSubscribe<TextureChangedEvent>(textureChangedSubscription);
	eventManager.UnSubscribe<IndexListChangedEvent>(indexListChangedSubscription);
}

Entity::HandleID TiledTerrain::AddTileMapLayer(const std::string& textureWrapperId)
{
	// Create the layer entity and add the according components
	auto& layer = entityManager.CreateEntity();
	layer.AddComponent<TransformComponent>();
	layer.AddComponent<RenderInformationComponent>(RenderLayer::Foreground);
	layer.AddComponent<TextureWrapperComponent>(textureWrapperId);
	layer.AddComponent<TiledTerrainLayerComponent>();
	layer.AddComponent<TiledTerrainLayerRenderComponent>(size, tileSize);
	eventManager.RaiseEvent(EntityCreatedEvent(layer.GetHandleID()));

	// Set the render objects zOrder to the number of tile map layers (not -1 since this layer has not been inserted yet)
	layer.GetComponent<RenderInformationComponent>().SetZOrder(static_cast<float>(renderLayerList.size()));

	renderLayerList.push_back(layer.GetHandleID());

	return layer.GetHandleID();
}

Entity::HandleID TiledTerrain::GetLayer(const size_t layerIndex)
{
	// If the index is invalid, return a LayerId for which no object will exist
	if (layerIndex >= renderLayerList.size())
		return Entity::GetNullID();

	return renderLayerList[layerIndex];
}

void TiledTerrain::SwopRenderLayerOrder(const size_t first, const size_t second)
{
	// Check wether the indecies are valid (the list of render nodes should always be the same size)
	if (first >= renderLayerList.size() || second >= renderLayerList.size())
		throw std::runtime_error("No tile map layer exists for this index");

	// Swopping does not make a difference
	if (first == second)
		return;

	assert(Entity::GetObjectFromID(renderLayerList[first]) != nullptr && "TiledTerrain: The layer entity must exists");
	assert(Entity::GetObjectFromID(renderLayerList[second]) != nullptr && "TiledTerrain: The layer entity must exists");

	auto& firstRenderInformationComponent = Entity::GetObjectFromID(renderLayerList[first])->GetComponent<RenderInformationComponent>();
	auto& secondRenderInformationComponent = Entity::GetObjectFromID(renderLayerList[second])->GetComponent<RenderInformationComponent>();

	// swop the zOrder
	auto temp = firstRenderInformationComponent.GetZOrder();
	firstRenderInformationComponent.SetZOrder(secondRenderInformationComponent.GetZOrder());
	secondRenderInformationComponent.SetZOrder(temp);
}

void TiledTerrain::RecalculateLayer(Entity& entity)
{
	// Recalculate the indecies
	try
	{
		entity.GetComponent<TiledTerrainLayerRenderComponent>().Create(entity.GetComponent<TiledTerrainLayerComponent>().GetIndexList());
	}
	catch (const std::runtime_error & error)
	{
		std::cerr << std::endl << error.what();
	}
}

void TiledTerrain::SubscribeEvents()
{
	// Subscribe to texture changed event
	textureChangedSubscription = eventManager.Subscribe(EventManager::TCallback<TextureChangedEvent>([this](const TextureChangedEvent& event)
		{
			if (!event.IsValueChanged("TextureWrapper"))
				return;
		}));

	// Subscribe to index list changed event
	indexListChangedSubscription = eventManager.Subscribe(EventManager::TCallback<IndexListChangedEvent>([this](const IndexListChangedEvent& event)
		{
			if (!event.IsValueChanged("IndexList"))
				return;
		}));
}

void TiledTerrain::OnTextureWrapperChangedEvent(TextureWrapperComponent& textureWraapperComponent)
{
	auto& entity = textureWraapperComponent.GetParentEntity();

	// Use event Subscription to update the texture
	if (!entity.HasComponent<TiledTerrainLayerRenderComponent>())
		return;

	// Update the render states
	// This is important to ensure that the texture with which the tiled terrain render component is created is up to date
	auto& renderComponent = entity.GetComponent<TiledTerrainLayerRenderComponent>();

	// Update the texture
	auto renderStates = renderComponent.GetRenderStates();

	renderStates.texture = &entity.GetComponent<TextureWrapperComponent>().GetTextureWrapper().GetTexture();

	// The move must be in the end
	renderComponent.SetRenderStates(std::move(renderStates));

	if (entity.HasComponent<TiledTerrainLayerComponent>())
		RecalculateLayer(entity);
}

void TiledTerrain::OnIndexListChangedEvent(TiledTerrainLayerComponent& tiledTerrainLayerComponent)
{
	auto& entity = tiledTerrainLayerComponent.GetParentEntity();

	// Use event Subscription to update the texture
	if (!entity.HasComponent<TiledTerrainLayerRenderComponent>())
		return;

	RecalculateLayer(entity);
}

void TiledTerrain::Data::Load(const std::string filePath)
{
	using namespace tinyxml2;

	// Load the document
	XMLDocument xmlDocument;
	if (xmlDocument.LoadFile(filePath.c_str()) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The file could not be loaded");

	// Get the root node
	const XMLNode* const rootNode = xmlDocument.FirstChild();
	if (rootNode == nullptr)
		throw std::runtime_error("Tile map data: The root node could not be found");

	// Get the map size and tile size (They are stored as attributes of the map node)
	//const XMLElement * mapElement = rootNode->ToElement();
	const XMLElement* mapElement = xmlDocument.FirstChildElement("map");
	if (mapElement->QueryUnsignedAttribute("width", &this->mapSize.x) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The map width could not be parsed");

	if (mapElement->QueryUnsignedAttribute("height", &this->mapSize.y) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The map height could not be parsed");

	if (mapElement->QueryUnsignedAttribute("tilewidth", &this->tileSize.x) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The tile width could not be parsed");

	if (mapElement->QueryUnsignedAttribute("tileheight", &this->tileSize.y) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The tile height could not be parsed");

	// Get the tile map layers
	//const XMLElement * tileMapLayerListElement = rootNode->ToElement(); // Does not work for some reason
	const XMLElement* const tileMapLayerListElement = xmlDocument.FirstChildElement("map");
	const XMLElement* layerElement = tileMapLayerListElement->FirstChildElement("layer");

	// For each tile map layer
	while (layerElement != nullptr)
	{
		// Get the data
		const XMLElement* layerDataElement = layerElement->FirstChildElement("data");
		if (layerDataElement == nullptr)
			throw std::runtime_error("Tile map data: The layer's data element could not be found");

		const char* layerData = layerDataElement->GetText();
		if (layerData == nullptr)
			throw std::runtime_error("Tile map data: The layer's data element must contain a list of tile indices");

		std::string layerDataString = layerData;


		// Append it to the tile map layer list
		this->ParseTileIndices(layerDataString);

		// Move to the next layer
		layerElement = layerElement->NextSiblingElement("layer");
	}
}

void TiledTerrain::Data::ParseTileIndices(std::string tileMapLayerData)
{
	std::vector<size_t> layerIndexList;

	std::string currentItemString;
	for (size_t i = 0u; i < tileMapLayerData.size(); i++)
	{
		// Get the substrings between two '\n' or ','
		const char currentChar = tileMapLayerData[i];
		if (currentChar == '\n' || currentChar == ',')
		{
			if (!currentItemString.empty())
			{
				/// What does this method return when it fails?
				int layerIndex = std::stoi(currentItemString);

				// Decrement the layer index to fit the mbe indices
				layerIndex--;

				layerIndexList.push_back(layerIndex);
			}

			// Reset the current item string
			currentItemString = "";
		}
		else
		{
			// Ignore white spaces
			if (currentChar == ' ')
				continue;

			// Add the current char to the current item string
			currentItemString += currentChar;
		}
	}

	// Add the layer to the tile map layer index list
	tileMapLayersIndexList.push_back(layerIndexList);
}
