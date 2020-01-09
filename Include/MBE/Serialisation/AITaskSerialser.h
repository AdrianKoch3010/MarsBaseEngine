#pragma once

/// @file
/// @brief class mbe::AITaskSerialiser

#include <memory>
#include <unordered_map>

#include <MBE/Core/TinyXML.h>
#include <MBE/AI/AITask.h>

namespace mbe
{
	/// @brief Interface for loading and storing ai task data from and to an XML file format
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
		virtual typename AITask::Ptr Load(const tinyxml2::XMLElement& taskData, float utility) = 0;

		virtual void Store(const typename AITask::Ptr aiTask, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) = 0;
	};


} // namespace mbe