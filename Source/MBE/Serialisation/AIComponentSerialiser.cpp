#include <MBE/Serialisation/AIComponentSerialiser.h>

using namespace mbe;

void AIComponentSerialser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	// Add the mbe::AIComponent
	auto& aiComponent = entity.AddComponent<UtilityAIComponent>();

	const auto activeTaskElement = componentData.FirstChildElement("ActiveTask");
	// The active task is optional
	if (activeTaskElement != nullptr)
	{
		// Get the type
		auto type = activeTaskElement->Attribute("type");
		const std::string typeString{ type };

		// Call the task serialiser for this type
		if (aiTaskSerialiserDictionary.find(typeString) == aiTaskSerialiserDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this type (" + typeString + ")");
		aiTaskSerialiserDictionary.at(typeString)->Load(aiComponent, *activeTaskElement);
	}

	const auto queuedTaskElement = componentData.FirstChildElement("QueuedTask");
	// The queued task is optional
	if (queuedTaskElement != nullptr)
	{
		// Get the type
		auto type = queuedTaskElement->Attribute("type");
		const std::string typeString{ type };

		// Call the task serialiser for this type
		if (aiTaskSerialiserDictionary.find(typeString) == aiTaskSerialiserDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this type (" + typeString + ")");
		aiTaskSerialiserDictionary.at(typeString)->Load(aiComponent, *queuedTaskElement);
	}
}

void AIComponentSerialser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	const auto& aiComponent = entity.GetComponent<UtilityAIComponent>();

	// Add the active task, if present
	if (aiComponent.IsTaskActive())
	{
		const auto typeId = aiComponent.GetActiveTaskTypeID();
		if (aiTaskTypeDictionary.find(typeId) == aiTaskTypeDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this active ai task type id");

		// Create the active task element
		auto activeTaskElement = document.NewElement("ActiveTask");
		activeTaskElement->SetAttribute("type", aiTaskTypeDictionary.at(typeId).c_str());

		// Store the task data
		aiTaskSerialiserDictionary.at(aiTaskTypeDictionary.at(typeId))->Store(aiComponent, document, *activeTaskElement);

		componentData.InsertEndChild(activeTaskElement);
	}

	// Add the queued task, if present
	if (aiComponent.IsTaskQueued())
	{
		const auto typeId = aiComponent.GetQueuedTaskTypeID();
		if (aiTaskTypeDictionary.find(typeId) == aiTaskTypeDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this queued ai task type id");

		// Create the active task element
		auto queuedTaskElement = document.NewElement("QueuedTask");
		queuedTaskElement->SetAttribute("type", aiTaskTypeDictionary.at(typeId).c_str());

		// Store the task data
		aiTaskSerialiserDictionary.at(aiTaskTypeDictionary.at(typeId))->Store(aiComponent, document, *queuedTaskElement);

		componentData.InsertEndChild(queuedTaskElement);
	}
}
