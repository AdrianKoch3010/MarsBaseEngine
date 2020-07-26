#pragma once

/// @file
/// @brief class mbe::AITaskSerialiser

#include <memory>
#include <unordered_map>

#include <MBE/Core/TinyXML.h>
#include <MBE/AI/AITask.h>
#include <MBE/AI/AIComponent.h>

namespace mbe
{
	/// @brief Interface for loading and storing ai task data from and to an XML file format
	class AITaskSerialiser
	{
	public:
		typedef std::shared_ptr<AITaskSerialiser> Ptr;
		typedef std::weak_ptr<AITaskSerialiser> WPtr;
		typedef std::unique_ptr<AITaskSerialiser> UPtr;

	public:
		AITaskSerialiser() = default;
		~AITaskSerialiser() = default;

	public:
		virtual AITask::Ptr Load(const tinyxml2::XMLElement& taskData, float utility) = 0;

		//virtual void Store(const typename AITask::Ptr aiTask, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) = 0;
		virtual void Store(const mbe::AIComponent& aiComponent, bool activeTask, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) = 0;
	};


} // namespace mbe