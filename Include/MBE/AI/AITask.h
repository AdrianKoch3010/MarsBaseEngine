#pragma once

///@file
///@brief Class mbe::AIState

#include <string>
#include <memory>

#include <MBE/Core/HandleBase.h>
#include <MBE/AI/AIAction.h>

namespace mbe
{

	///@brief Base class for AIStates managed by the mbe::AIComponent
	///@details An mbe::AIState contains the information on an action performed by the an mbe::Entity
	class AITask : public HandleBase<AITask>
	{
	public:
		typedef std::shared_ptr<AITask> Ptr;
		typedef std::weak_ptr<AITask> WPtr;
		typedef std::unique_ptr<AITask> UPtr;

	public:
		///@brief Constructor
		AITask(float utility);

		///@brief Default Destructor
		virtual ~AITask() = default;

	public:
		virtual void Update() = 0;

		virtual void Abort() = 0;

		inline float GetUtility() const { return utility; }

		inline bool IsAborting() const { return isAborting; }

		inline void SetUtilty(float value) { utility = value; }

		inline void SetAborting(bool value = true) { isAborting = value; }
	
	protected:
		float utility;
		bool isAborting;
	};

} // namespace mbe