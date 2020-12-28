#include <MBE/Serialisation/AIComponentSerialiser.h>

using namespace mbe;

void AIComponentSerialser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
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
		auto taskPtr = AITaskSerialiserRegistry::Instance()[typeString].Load(*activeTaskElement, utility);

		// Set the active task under this type
		const auto aiTaskTypeId = AITaskSerialiserRegistry::Instance().GetObjectTypeByName(typeString);
		aiComponent.SetActiveTask({ aiTaskTypeId, taskPtr });
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
		auto taskPtr = AITaskSerialiserRegistry::Instance()[typeString].Load(*queuedTaskElement, utility);

		// Set the active task under this type
		const auto aiTaskTypeId = AITaskSerialiserRegistry::Instance().GetObjectTypeByName(typeString);
		aiComponent.SetQueuedTask({ aiTaskTypeId, taskPtr });
	}
}

void AIComponentSerialser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have an ai component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<AIComponent>() == false)
		throw std::runtime_error("Store ai component: The entity must have an mbe::AIComponent");
	const auto& aiComponent = entity.GetComponent<AIComponent>();

	// Add the active task, if present
	if (aiComponent.IsTaskActive())
	{
		const auto typeId = aiComponent.GetActiveTaskTypeID();

		// Create the active task element
		auto activeTaskElement = document.NewElement("ActiveTask");
		activeTaskElement->SetAttribute("type", AITaskSerialiserRegistry::Instance().GetObjectName(typeId).c_str());
		activeTaskElement->SetAttribute("utility", aiComponent.GetActiveTask().GetUtility());

		// Store the task data
		AITaskSerialiserRegistry::Instance()[typeId].Store(aiComponent, true, document, *activeTaskElement);

		componentData.InsertEndChild(activeTaskElement);
	}

	// Add the queued task, if present
	if (aiComponent.IsTaskQueued())
	{
		const auto typeId = aiComponent.GetQueuedTaskTypeID();

		// Create the active task element
		auto queuedTaskElement = document.NewElement("QueuedTask");
		queuedTaskElement->SetAttribute("type", AITaskSerialiserRegistry::Instance().GetObjectName(typeId).c_str());
		queuedTaskElement->SetAttribute("utility", aiComponent.GetQueuedTask().GetUtility());

		// Store the task data
		AITaskSerialiserRegistry::Instance()[typeId].Store(aiComponent, false, document, *queuedTaskElement);

		componentData.InsertEndChild(queuedTaskElement);
	}
}
