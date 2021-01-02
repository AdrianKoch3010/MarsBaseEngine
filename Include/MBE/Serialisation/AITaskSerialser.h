#pragma once

/// @file
/// @brief class mbe::AITaskSerialiser

#include <memory>
#include <unordered_map>

#include <MBE/Core/TinyXML.h>

#include <MBE/Core/Exceptions.h>
#include <MBE/Core/Utility.h>
#include <MBE/AI/AITask.h>
#include <MBE/AI/AIComponent.h>

MBE_DECLARE_TYPE_ID(AITaskSerialiser)

namespace mbe
{
	/// @brief Interface for loading and storing ai task data from and to an XML file format
	class AITaskSerialiser
	{
	public:
		typedef std::shared_ptr<AITaskSerialiser> Ptr;
		typedef std::weak_ptr<AITaskSerialiser> WPtr;
		typedef std::unique_ptr<AITaskSerialiser> UPtr;

		typedef detail::AITaskSerialiserTypeID TypeID;

	public:
		AITaskSerialiser() = default;
		~AITaskSerialiser() = default;

	public:
		virtual AITask::Ptr Load(const tinyxml2::XMLElement& taskData, float utility) const = 0;

		//virtual void Store(const typename AITask::Ptr aiTask, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) = 0;
		virtual void Store(const mbe::AIComponent& aiComponent, bool activeTask, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& taskData) const = 0;

		MBE_GET_TYPE_ID(AITaskSerialiser)

	};


} // namespace mbe