#pragma once

/// @file
/// @brief class mbe::AITaskSerialiser

#include <memory>
#include <unordered_map>

#include <MBE/Core/TinyXML.h>
#include <MBE/AI/AIComponent.h>

namespace mbe
{

	class AITaskSerialiser
	{
	public:
		std::shared_ptr<AITaskSerialiser> Ptr;
		std::weak_ptr<AITaskSerialiser> WPtr;
		std::unique_ptr<AITaskSerialiser> UPtr;

	public:
		AITaskSerialiser() = default;
		~AITaskSerialiser() = default;

	public:
		virtual void Load(UtilityAIComponent& aiComponent, const tinyxml2::XMLElement& taskData) = 0;

		virtual void Store(const UtilityAIComponent& aiComponent, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) = 0;
	};


} // namespace mbe