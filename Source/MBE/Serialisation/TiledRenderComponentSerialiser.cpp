#include <MBE/Graphics/TiledRenderComponent.h>

#include <MBE/Serialisation/TiledRenderComponentSerialiser.h>

using namespace mbe;

void TiledRenderComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	bool isCreated;
	sf::Vector2u size, tileSize;

	// Get created
	const auto createdElement = componentData.FirstChildElement("Created");
	if (createdElement == nullptr)
		throw std::runtime_error("Load tiled render component: Failed to parse created");
	if (createdElement->QueryBoolText(&isCreated) != XML_SUCCESS)
		throw std::runtime_error("Load tiled render component: Failed to parse created text");

	// Get size
	const auto sizeElement = componentData.FirstChildElement("Size");
	if (sizeElement == nullptr)
		throw std::runtime_error("Load tiled render component: Failed to parse size");
	if (sizeElement->QueryUnsignedAttribute("x", &size.x) != XML_SUCCESS)
		throw std::runtime_error("Load tiled render component: Failed to parse size x attribute");
	if (sizeElement->QueryUnsignedAttribute("y", &size.y) != XML_SUCCESS)
		throw std::runtime_error("Load tiled render component: Failed to parse size y attribute");

	// Get tile size
	const auto tileSizeElement = componentData.FirstChildElement("TileSize");
	if (tileSizeElement == nullptr)
		throw std::runtime_error("Load tiled render component: Failed to parse tile size");
	if (tileSizeElement->QueryUnsignedAttribute("x", &tileSize.x) != XML_SUCCESS)
		throw std::runtime_error("Load tiled render component: Failed to parse tile size x attribute");
	if (tileSizeElement->QueryUnsignedAttribute("y", &tileSize.y) != XML_SUCCESS)
		throw std::runtime_error("Load tiled render component: Failed to parse tile size y attribute");

	auto& tiledRenderComponent = entity.AddComponent<TiledRenderComponent>(size, tileSize);
}

void TiledRenderComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have a tiled render component (this should be the case when this function is called from the EntitySerialiser)
	if (entity.HasComponent<TiledRenderComponent>() == false)
		throw std::runtime_error("Store tiled render component: The entity must have an mbe::TiledRenderComponent");

	const auto& tiledRenderComponent = entity.GetComponent<TiledRenderComponent>();

	// Store created
	auto createdElement = document.NewElement("Created");
	createdElement->SetText(tiledRenderComponent.IsCreated());
	componentData.InsertEndChild(createdElement);

	// Store size
	auto sizeElement = document.NewElement("Size");
	sizeElement->SetAttribute("x", tiledRenderComponent.GetSize().x);
	sizeElement->SetAttribute("y", tiledRenderComponent.GetSize().y);
	componentData.InsertEndChild(sizeElement);

	// Store tile size
	auto tileSizeElement = document.NewElement("TileSize");
	tileSizeElement->SetAttribute("x", tiledRenderComponent.GetTileSize().x);
	tileSizeElement->SetAttribute("y", tiledRenderComponent.GetTileSize().y);
	componentData.InsertEndChild(tileSizeElement);
}
