#include "..\..\..\Include\MBE\Serialisation\EntitySerialiser.h"
#include <MBE/Serialisation/EntitySerialiser.h>
#include <MBE/Serialisation/SerialiserRegistry.h>

using namespace mbe;

EntitySerialiser::EntitySerialiser(EntityManager& entityManager, EventManager& eventManager/*, Context context*/) :
	entityManager(entityManager),
	eventManager(eventManager)
	//context(context)
{
}

std::vector<Entity::HandleID> EntitySerialiser::LoadEntities(const std::string& filePath)
{
	using namespace tinyxml2;

	// Load the XML file
	XMLDocument document;
	XMLError loadError = document.LoadFile(filePath.c_str());
	if (loadError != XML_SUCCESS)
		throw std::runtime_error("Load Entities: Error while reading from xml file (" + filePath + ") error id: " + std::to_string(loadError));

	return Load(document);
}

std::vector<Entity::HandleID> EntitySerialiser::TryLoadEntities(const std::string& filePath, const std::string& fileName)
{
	try
	{
		return LoadEntities(filePath + fileName);
	}
	catch (const mbe::ParseError& parseError)
	{
		std::cerr << filePath << fileName << ": " << parseError.GetParser() << ":" << std::endl;
		std::cerr << fileName;
		if (parseError.GetLineNumber() > 0)
			std::cerr << ":" << parseError.GetLineNumber();
		std::cerr << ": " << parseError.GetMessage() << std::endl;
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << "\nFailed to parse "<< filePath << fileName << error.what() << std::endl;
	}

	// Return an empty vector
	return std::vector<Entity::HandleID>();
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
		for (const auto componentTypeId : entity.GetComponentTypeIDList())
		{
			// If there is a component serialiser for this component, use it
			// Otherwise, do nothing
			if (ComponentSerialiserRegistry::Instance().HasSerialiser(componentTypeId))
			{
				// There will always be a component serialiser for this type
				auto componentElement = document.NewElement("Component");
				componentElement->SetAttribute("type", ComponentSerialiserRegistry::Instance().GetObjectName(componentTypeId).c_str());

				// Call the corresponding component serialiser
				ComponentSerialiserRegistry::Instance()[componentTypeId].StoreComponent(entity, document, *componentElement);
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
		throw ParseError(MBE_NAME_OF(EntitySerialiser), "The root node could not be found", document.GetLineNum());

	// Load the list of entities
	for (auto entityElement = rootNode->FirstChildElement("Entity"); entityElement != nullptr; entityElement = entityElement->NextSiblingElement("Entity"))
	{
		// Get the entity id
		int64_t entityId;
		if (entityElement->QueryInt64Attribute("id", &entityId) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(EntitySerialiser), "Failed to parse Entity id attribute",entityElement->GetLineNum());

		// Get the parent entity id
		int64_t parentEntityId;
		if (entityElement->QueryInt64Attribute("parentId", &parentEntityId) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(EntitySerialiser), "Failed to parse Entity parentId attribute", entityElement->GetLineNum());

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
				throw ParseError(MBE_NAME_OF(EntitySerialiser), "Failed to parse Component type attribute", componentElement->GetLineNum());
			std::string componentTypeString{ componentType };

			try
			{
				ComponentSerialiserRegistry::Instance()[componentTypeString].LoadComponent(entity, *componentElement);
			}
			// This is so that the parse error is not caught by the runtime_error
			catch (const ParseError& parseError)
			{
				throw parseError;
			}
			catch (const std::runtime_error& error)
			{
				throw ParseError(MBE_NAME_OF(EntitySerialiser), "Unknown component serialiser (" + componentTypeString + ")", componentElement->GetLineNum());
			}
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
