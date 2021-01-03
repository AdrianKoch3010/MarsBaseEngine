#include <MBE/Serialisation/AIComponentSerialiser.h>

using namespace mbe;

void AIComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
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
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Failed to parse ActiveTask type attribute", activeTaskElement->GetLineNum());
		const std::string typeString{ type };

		// Get the utility
		float utility;
		if (activeTaskElement->QueryFloatAttribute("utility", &utility) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Failed to parse ActiveTask utility attribute", activeTaskElement->GetLineNum());

		// Call the task serialiser for this type which creates the task
		try
		{
			auto taskPtr = AITaskSerialiserRegistry::Instance()[typeString].Load(*activeTaskElement, utility);

			// Set the active task under this type
			const auto aiTaskTypeId = AITaskSerialiserRegistry::Instance().GetObjectTypeByName(typeString);
			aiComponent.SetActiveTask({ aiTaskTypeId, taskPtr });
		}
		// This is so that the parse error is not caught by the runtime_error
		catch (const ParseError& parseError)
		{
			throw parseError;
		}
		catch (const std::runtime_error& error)
		{
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Unkown AITask serialiser (" + typeString + ")", activeTaskElement->GetLineNum());
		}
	}

	const auto queuedTaskElement = componentData.FirstChildElement("QueuedTask");
	// The queued task is optional
	if (queuedTaskElement != nullptr)
	{
		// Get the type
		const auto type = queuedTaskElement->Attribute("type");
		if (type == nullptr)
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Failed to parse QueuedTask type attribute", queuedTaskElement->GetLineNum());
		const std::string typeString{ type };

		// Get the utility
		float utility;
		if (queuedTaskElement->QueryFloatAttribute("utility", &utility) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Failed to parse QueuedTask utility attribute", queuedTaskElement->GetLineNum());

		// Call the task serialiser for this type which creates the task
		try
		{
			auto taskPtr = AITaskSerialiserRegistry::Instance()[typeString].Load(*queuedTaskElement, utility);

			// Set the active task under this type
			const auto aiTaskTypeId = AITaskSerialiserRegistry::Instance().GetObjectTypeByName(typeString);
			aiComponent.SetQueuedTask({ aiTaskTypeId, taskPtr });
		}
		// This is so that the parse error is not caught by the runtime_error
		catch (const ParseError& parseError)
		{
			throw parseError;
		}
		catch (const std::runtime_error& error)
		{
			throw ParseError(MBE_NAME_OF(AIComponentSerialiser), "Unkown AITask serialiser (" + typeString + ")", queuedTaskElement->GetLineNum());
		}
	}
}

void AIComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
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
