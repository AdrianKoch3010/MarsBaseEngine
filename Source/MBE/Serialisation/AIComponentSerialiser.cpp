#include <MBE/Serialisation/AIComponentSerialiser.h>

// Only for testing

using namespace mbe;

AIComponentSerialser::AITaskSerialiserDictionary AIComponentSerialser::aiTaskSerialiserDictionary;
AIComponentSerialser::AITaskTypeDictionary AIComponentSerialser::aiTaskTypeDictionary;
AIComponentSerialser::AITaskTypeStringDictionary AIComponentSerialser::aITaskTypeStringDictionary;

void AIComponentSerialser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	using namespace tinyxml2;

	// Add the mbe::AIComponent
	auto& aiComponent = entity.AddComponent<AIComponent>();

	const auto activeTaskElement = componentData.FirstChildElement("ActiveTask");
	// The active task is optional
	if (activeTaskElement != nullptr)
	{
		// Get the type
		const auto type = activeTaskElement->Attribute("type");
		if (type == nullptr)
			throw std::runtime_error("Ai component serialiser: Failed to parse ai task type attribute");
		const std::string typeString{ type };

		// Get the utility
		float utility;
		if (activeTaskElement->QueryFloatAttribute("utility", &utility) != XML_SUCCESS)
			throw std::runtime_error("Ai component serialiser: Failed to parse ai task utility attribute");

		// Call the task serialiser for this type which creates the task
		if (aiTaskSerialiserDictionary.find(typeString) == aiTaskSerialiserDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this type (" + typeString + ")");
		auto taskPtr = aiTaskSerialiserDictionary.at(typeString)->Load(*activeTaskElement, utility);

		// Set the active task under this type
		aiComponent.SetActiveTask({ aITaskTypeStringDictionary.at(typeString), taskPtr });
	}

	const auto queuedTaskElement = componentData.FirstChildElement("QueuedTask");
	// The queued task is optional
	if (queuedTaskElement != nullptr)
	{
		// Get the type
		const auto type = queuedTaskElement->Attribute("type");
		if (type == nullptr)
			throw std::runtime_error("Ai component serialiser: Failed to parse ai task type attribute");
		const std::string typeString{ type };

		// Get the utility
		float utility;
		if (queuedTaskElement->QueryFloatAttribute("utility", &utility) != XML_SUCCESS)
			throw std::runtime_error("Ai component serialiser: Failed to parse ai task utility attribute");

		// Call the task serialiser for this type which creates the task
		if (aiTaskSerialiserDictionary.find(typeString) == aiTaskSerialiserDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this type (" + typeString + ")");
		auto taskPtr = aiTaskSerialiserDictionary.at(typeString)->Load(*queuedTaskElement, utility);

		// Set the active task under this type
		aiComponent.SetQueuedTask({ aITaskTypeStringDictionary.at(typeString), taskPtr });
	}
}

void AIComponentSerialser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	// The entity must have an ai component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<AIComponent>() == false)
		throw std::runtime_error("Store ai component: The entity must have an mbe::AIComponent");
	const auto& aiComponent = entity.GetComponent<AIComponent>();

	// Add the active task, if present
	if (aiComponent.IsTaskActive())
	{
		const auto typeId = aiComponent.GetActiveTaskTypeID();
		if (aiTaskTypeDictionary.find(typeId) == aiTaskTypeDictionary.end())
			throw std::runtime_error("Ai component serialiser: No task serialiser found for this active ai task type id");

		// Create the active task element
		auto activeTaskElement = document.NewElement("ActiveTask");
		activeTaskElement->SetAttribute("type", aiTaskTypeDictionary.at(typeId).c_str());
		activeTaskElement->SetAttribute("utility", aiComponent.GetActiveTask().GetUtility());

		// Store the task data
		aiTaskSerialiserDictionary.at(aiTaskTypeDictionary.at(typeId))->Store(aiComponent, true, document, *activeTaskElement);

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
		queuedTaskElement->SetAttribute("utility", aiComponent.GetQueuedTask().GetUtility());

		// Store the task data
		aiTaskSerialiserDictionary.at(aiTaskTypeDictionary.at(typeId))->Store(aiComponent, false, document, *queuedTaskElement);

		componentData.InsertEndChild(queuedTaskElement);
	}
}
