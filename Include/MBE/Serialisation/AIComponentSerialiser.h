#pragma once

/// @file
/// @brief AIComponentSerialiser

#include <string>
#include <unordered_map>
#include <memory>

#include <MBE/Serialisation/SerrialiserRegistry.h>
#include <MBE/AI/AIComponent.h>
#include <MBE/Serialisation/AITaskSerialser.h>

namespace mbe
{
	// Both the active and queued task is optional
	// <Component type="AIComponent">
	//		<ActiveTask type="string" utility="float">
	//			// Task data
	//		</ActiveTask type="string">
	//		<QueuedTask type="string" utility="float">
	//			// Task data
	//		</QueuedTask>
	// </Component>
	class AIComponentSerialser : public ComponentSerialiser
	{
	public:
		std::shared_ptr<AIComponentSerialser> Ptr;
		std::weak_ptr<AIComponentSerialser> WPtr;
		std::unique_ptr<AIComponentSerialser> UPtr;

	public:
		AIComponentSerialser() = default;
		~AIComponentSerialser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;

	};

} // namespace mbe