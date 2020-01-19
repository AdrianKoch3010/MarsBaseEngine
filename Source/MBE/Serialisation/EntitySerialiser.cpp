// Component serialisers
#include <MBE/Serialisation/TransformComponentSerialiser.h>
#include <MBE/Serialisation/AIComponentSerialiser.h>
#include <MBE/Serialisation/AnimationComponentSerialser.h>
#include <MBE/Serialisation/PixelMaskClickableComponentSerialiser.h>
#include <MBE/Serialisation/TopDownInformationComponentSerialiser.h>
#include <MBE/Serialisation/RenderInformationComponentSerialiser.h>
#include <MBE/Serialisation/TextureWrapperComponentSerialiser.h>
#include <MBE/Serialisation/TiledRenderComponentSerialiser.h>
#include <MBE/Serialisation/SpriteRenderComponentSerialiser.h>
#include <MBE/Serialisation/TileMapComponentSerialiser.h>

// Components
#include <MBE/TransformComponent.h>
#include <MBE/AI/AIComponent.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Input/PixelMaskClickableComponent.h>
#include <MBE/Graphics/TopDownInformationComponent.h>
#include <MBE/Graphics/RenderInformationComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/Graphics/TiledRenderComponent.h>
#include <MBE/Graphics/SpriteRenderComponent.h>
#include <MBE/Map/TileMapComponent.h>

#include <MBE/Serialisation/EntitySerialiser.h>

using namespace mbe;

EntitySerialiser::EntitySerialiser(EntityManager& entityManager, EventManager& eventManager/*, Context context*/) :
	entityManager(entityManager),
	eventManager(eventManager)
	//context(context)
{
	AddComponentSerialiser<TransformComponentSerialser, TransformComponent>("TransformComponent");
	AddComponentSerialiser<AIComponentSerialser, AIComponent>("AIComponent");
	AddComponentSerialiser<AnimationComponentSerialiser, AnimationComponent>("AnimationComponent");
	AddComponentSerialiser<PixelMaskClickableComponentSerialiser, PixelMaskClickableComponent>("PixelMaskClickableComponent");
	AddComponentSerialiser<TopDownInformationComponentSerialiser, TopDownInformationComponent>("TopDownInformationComponent");
	AddComponentSerialiser<RenderInformationComponentSerialiser, RenderInformationComponent>("RenderInformationComponent");
	AddComponentSerialiser<TextureWrapperComponentSerialiser, TextureWrapperComponent>("TextureWrapperComponent");
	AddComponentSerialiser<TiledRenderComponentSerialiser, TiledRenderComponent>("TiledRenderComponent");
	AddComponentSerialiser<SpriteRenderComponentSerialiser, SpriteRenderComponent>("SpriteRenderComponent");
	AddComponentSerialiser<TileMapComponentSerialiser, TileMapComponent>("TileMapComponent");
}

std::vector<Entity::HandleID> EntitySerialiser::LoadEntites(const std::string& filePath)
{
	using namespace tinyxml2;

	// Load the XML file
	XMLDocument document;
	XMLError loadError = document.LoadFile(filePath.c_str());
	if (loadError != XML_SUCCESS)
		throw std::runtime_error("Load Entities: Error while reading from xml file (" + filePath + ") error id: " + std::to_string(loadError));

	return Load(document);
}

std::vector<Entity::HandleID> EntitySerialiser::CreateEntitiesFromString(const std::string& xmlString)
{
	using namespace tinyxml2;

	// Parse the xml string
	XMLDocument document;
	XMLError parseError = document.Parse(xmlString.c_str());
	if (parseError != XML_SUCCESS)
		throw std::runtime_error("Load Entities: Error parsing xml string error id:" + std::to_string(parseError));

	return Load(document);
}

void EntitySerialiser::StoreEntites(const std::string& filePath)
{
	using namespace tinyxml2;

	XMLDocument document;
	auto rootNode = document.NewElement("Entities");
	document.InsertFirstChild(rootNode);

	for (const auto entityId : entityManager.GetEntityIDList())
	{
		// The entity must exists
		assert(Entity::GetObjectFromID(entityId) != nullptr && "Store entities: The entity must exist");

		// Create the entity
		const auto& entity = *Entity::GetObjectFromID(entityId);
		auto entityElement = document.NewElement("Entity");
		entityElement->SetAttribute("id", static_cast<int64_t>(entity.GetHandleID()));
		entityElement->SetAttribute("parentId", static_cast<int64_t>(entity.GetParentEntityID()));
		rootNode->InsertEndChild(entityElement);

		// Store the components
		for (auto componentTypeId : entity.GetComponentTypeIDList())
		{
			// If there is a component serialiser for this component, use it
			// Otherwise, do nothing
			if (componentTypeDictionary.count(componentTypeId))
			{
				// There will always be a component serialiser for this type
				auto componentElement = document.NewElement("Component");
				componentElement->SetAttribute("type", componentTypeDictionary.at(componentTypeId).c_str());

				// Call the corresponding component serialiser
				componentSerialiserDictionary.at(componentTypeDictionary.at(componentTypeId))->StoreComponent(entity, document, *componentElement);
				entityElement->InsertEndChild(componentElement);
			}
		}
	}

	// Save it to the file
	XMLError storeError = document.SaveFile(filePath.c_str());

	if (storeError != XMLError::XML_SUCCESS)
		throw std::runtime_error("Store Entities: Error while writing to xml file (" + std::to_string(storeError) + ")");
}

std::vector<Entity::HandleID> EntitySerialiser::Load(const tinyxml2::XMLDocument& document)
{
	using namespace tinyxml2;

	// Remember the entities that have been added
	std::vector<Entity::HandleID> loadedEntityIdList;

	// Get the root node
	const auto rootNode = document.FirstChild();
	if (rootNode == nullptr)
		throw std::runtime_error("Load Entities: The root node could not be found");

	// Load the list of entities
	for (auto entityElement = rootNode->FirstChildElement("Entity"); entityElement != nullptr; entityElement = entityElement->NextSiblingElement("Entity"))
	{
		// Get the entity id
		int64_t entityId;
		if (entityElement->QueryInt64Attribute("id", &entityId) != XML_SUCCESS)
			throw std::runtime_error("Load Entities: Failed to parse entity id");

		// Get the parent entity id
		int64_t parentEntityId;
		if (entityElement->QueryInt64Attribute("parentId", &parentEntityId) != XML_SUCCESS)
			throw std::runtime_error("Load Entities: Failed to parse parent entity id");

		// Create the entity
		auto& entity = entityManager.CreateEntity();
		loadedEntityIdList.push_back(entity.GetHandleID());

		//////////////////////////// TODO:
		// Maping of new and old entity ids
		entityIdMap.insert({ static_cast<Entity::HandleID>(entityId), entity.GetHandleID() });
		parentEntityIdMap.insert({ entity.GetHandleID(), static_cast<Entity::HandleID>(parentEntityId) });

		// Maybe add entityManager.CreateLoadedEntity(mbe::Entity::HandleID id);

		// Get and attach the components
		for (auto componentElement = entityElement->FirstChildElement("Component"); componentElement != nullptr; componentElement = componentElement->NextSiblingElement("Component"))
		{
			// Get the component type
			auto componentType = componentElement->Attribute("type");
			if (componentType == nullptr)
				throw std::runtime_error("Load Components: Failed to parse component type");
			std::string componentTypeString{ componentType };

			// Call the appropreate component serialiser for that component type
			if (componentSerialiserDictionary.find(componentTypeString) == componentSerialiserDictionary.end())
				throw std::runtime_error("Load Components: No component serialiser found for this type (" + componentTypeString + ")");
			componentSerialiserDictionary.at(componentTypeString)->LoadComponent(entity, *componentElement);
		}
		// Raise the entity created event
		eventManager.RaiseEvent(event::EntityCreatedEvent(entity.GetHandleID()));
	}

	// Set the parent entities
	for (auto entityId : loadedEntityIdList)
	{
		auto& entity = *Entity::GetObjectFromID(entityId);
		// Get the old parent entity id for this entity
		// Then map it on the new entity and set it as a parent

		// Only set existing parent entities
		const auto parentEntityId = parentEntityIdMap.at(entityId);
		if (entityIdMap.find(parentEntityId) != entityIdMap.end())
			entity.SetParentEntityID(entityIdMap.at(parentEntityId));
	}

	return loadedEntityIdList;
}
