#pragma once

/// @file
/// @brief

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Serialisation/AITaskSerialser.h>
#include <MBE/AI/AIComponent.h>

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
	class AIComponentSerialser : public ComponentSerialser
	{
	public:
		std::shared_ptr<AIComponentSerialser> Ptr;
		std::weak_ptr<AIComponentSerialser> WPtr;
		std::unique_ptr<AIComponentSerialser> UPtr;

	private:
		typedef std::unordered_map<std::string, std::unique_ptr<AITaskSerialiser>> AITaskSerialiserDictionary;
		typedef std::unordered_map<typename AIComponent::AITaskTypeID, std::string> AITaskTypeDictionary;
		typedef std::unordered_map<std::string, typename AIComponent::AITaskTypeID> AITaskTypeStringDictionary;

	public:
		AIComponentSerialser() = default;
		~AIComponentSerialser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

	public:
		template<class TAITaskSerialiser, class TAITask, typename... TArguments>
		void AddAITaskSerialiser(const std::string& aiTaskType, TArguments... arguments);

	private:
		AITaskSerialiserDictionary aiTaskSerialiserDictionary;
		AITaskTypeDictionary aiTaskTypeDictionary;
		AITaskTypeStringDictionary aITaskTypeStringDictionary;
	};

#pragma region Template Implementations

	template<class TAITaskSerialiser, class TAITask, typename ...TArguments>
	inline void AIComponentSerialser::AddAITaskSerialiser(const std::string& aiTaskType, TArguments ...arguments)
	{
		// make sure that TAITaskSerialiser inherits from AITaskSerialiser
		static_assert(std::is_base_of<AITaskSerialiser, TAITaskSerialiser>::value, "The ai task serialiser must inherit from mbe::AITaskSerialser");

		// Throw if a component serialiser for this type already exists
		if (aiTaskSerialiserDictionary.find(aiTaskType) != aiTaskSerialiserDictionary.end())
			throw std::runtime_error("AIComponentSerialiser: An ai task serialser already exists for this task type (" + aiTaskType + ")");

		// Remember the typeId for this ai task type for the serialiser store function
		aiTaskTypeDictionary.insert({ detail::GetAITaskTypeID<TAITask>(), aiTaskType });

		// Remember the type string for this tyep
		aITaskTypeStringDictionary.insert({ aiTaskType, detail::GetAITaskTypeID<TAITask>() });

		// Make a new component serialser
		auto aiTaskSerialiserPtr = std::make_unique<TAITaskSerialiser>(std::forward<TArguments>(arguments)...);

		// Add the component serialiser to the dictionary
		aiTaskSerialiserDictionary.insert({ aiTaskType, std::move(aiTaskSerialiserPtr) });
	}

#pragma endregion


} // namespace mbe