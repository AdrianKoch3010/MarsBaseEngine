#include <MBE/TiledTerrain.h>

using namespace mbe;

TiledTerrain::TiledTerrain(EventManager & eventManager, EntityManager & entityManager, sf::Vector2u size, sf::Vector2u tileSize) :
	eventManager(eventManager),
	entityManager(entityManager),
	tiledTerrainLayerSystem(eventManager),
	size(size),
	tileSize(tileSize)
{
}

TiledTerrain::TiledTerrain(EventManager & eventManager, EntityManager & entityManager, Data::Ptr mapData, const sf::Texture & tileMapTexture) :
	eventManager(eventManager),
	entityManager(entityManager),
	tiledTerrainLayerSystem(eventManager),
	size(mapData->GetMapSize()),
	tileSize(mapData->GetTileSize())
{
	// Add layers
	for (const auto & layer : mapData->GetTileMapLayersIndexList())
	{
		Entity::HandleID layerId = this->AddTileMapLayer(tileMapTexture);
		/// Check if exists
		Entity::GetObjectFromID(layerId)->GetComponent<TiledTerrainLayerComponent>().SetTileIndexList(layer);
		eventManager.RaiseEvent(event::EntityCreatedEvent(layerId));
		//TiledTerrainLayer::GetObjectFromID(layerId)->Create(layer);
	}
}

Entity::HandleID TiledTerrain::AddTileMapLayer(const sf::Texture & texture)
{
	// Create the layer and renderNode
	//std::unique_ptr<TiledTerrainLayer> layer(new TiledTerrainLayer(texture, size, tileSize)); // Raw new since std::make_unique cannot access protected constructor

	//auto renderNode = std::make_unique<RenderNode>(*layer, RenderNode::Layer::Foreground);

	// Create the layer entity and add the according components
	auto & layer = entityManager.CreateEntity();
	layer.AddComponent<TransformComponent>();
	layer.AddComponent<TiledTerrainLayerComponent>(size, tileSize);
	layer.AddComponent<TiledTerrainLayerRenderComponent>(texture);

	// Set the render nodes zOrder to the number of tile map layers (not -1 since this layer has not been inserted yet)
	//renderNode->SetZOrder(tileMapLayerList.size());
	layer.GetComponent<TiledTerrainLayerRenderComponent>().SetZOrder(static_cast<float>(layerList.size()));


	// Raise the renderNodeCreated event
	//eventManager.RaiseEvent(mbe::event::EntityCreatedEvent(renderNode->GetHandleID()));

	// Get the layer id so that it can be returned
	//LayerID layerId = layer->GetHandleID();

	// Insert the layer and renderNode in order to expand their live span (until this object gets deleted)
	//tileMapLayerRenderNodeList.push_back(std::move(renderNode));
	//tileMapLayerList.push_back(std::move(layer));

	layerList.push_back(layer.GetHandleID());

	return layer.GetHandleID();
}

Entity::HandleID TiledTerrain::GetLayer(const size_t layerIndex)
{
	// If the index is invalid, return a LayerId for which no object will exist
	if (layerIndex >= layerList.size())
		return Entity::GetNullID();

	return layerList[layerIndex];
}

void TiledTerrain::SwopRenderLayerOrder(const size_t first, const size_t second)
{
	// Check wether the indecies are valid (the list of render nodes should always be the same size)
	if (first >= layerList.size() || second >= layerList.size())
		throw std::runtime_error("No tile map layer exists for this index");

	// Swopping does not make a difference
	if (first == second)
		return;

	assert(Entity::GetObjectFromID(layerList[first]) != nullptr && "TiledTerrain: The layer entity must exists");
	assert(Entity::GetObjectFromID(layerList[second]) != nullptr && "TiledTerrain: The layer entity must exists");

	auto & firstRenderComponent = Entity::GetObjectFromID(layerList[first])->GetComponent<TiledTerrainLayerRenderComponent>();
	auto & secondRenderComponent = Entity::GetObjectFromID(layerList[second])->GetComponent<TiledTerrainLayerRenderComponent>();

	// swop the zOrder
	auto temp = firstRenderComponent.GetZOrder();
	firstRenderComponent.SetZOrder(secondRenderComponent.GetZOrder());
	secondRenderComponent.SetZOrder(temp);
}

void TiledTerrain::Data::Load(const std::string filePath)
{
	using namespace tinyxml2;

	// Load the document
	XMLDocument xmlDocument;
	if (xmlDocument.LoadFile(filePath.c_str()) != XML_SUCCESS)
		throw std::runtime_error("Tile map data: The file could not be loaded");

	// Get the root node
	const XMLNode * rootNode = xmlDocument.FirstChild();
	if (rootNode == nullptr)
		throw std::runtime_error("Tile map data: No root node could be found");

	// Get the map size and tile size (They are stored as attributes of the map node)
	//const XMLElement * mapElement = rootNode->ToElement();
	const XMLElement * mapElement = xmlDocument.FirstChildElement("map");
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
	const XMLElement * tileMapLayerListElement = xmlDocument.FirstChildElement("map");
	const XMLElement * layerElement = tileMapLayerListElement->FirstChildElement("layer");

	// For each tile map layer
	while (layerElement != nullptr)
	{
		// Get the data
		const XMLElement * layerDataElement = layerElement->FirstChildElement("data");
		if (layerDataElement == nullptr)
			throw std::runtime_error("Tile map data: The layer's data element could not be found");

		const char * layerData = layerDataElement->GetText();
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
