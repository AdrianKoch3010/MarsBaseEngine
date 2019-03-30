#pragma once

///@file
///@brief Class mbe::AIState

#include <string>
#include <memory>

namespace mbe
{

	///@brief Base class for AIStates managed by the mbe::AIComponent
	///@details An mbe::AIState contains the information on an action performed by the an mbe::Entity
	class AIState
	{
	public:
		typedef std::shared_ptr<AIState> Ptr;
		typedef std::weak_ptr<AIState> WPtr;
		typedef std::unique_ptr<AIState> UPtr;

		typedef std::string StateID;

	public:
		///@brief Default Constructor
		AIState() = default;

		///@brief Default Destructor
		virtual ~AIState() = default;

	public:
		/*virtual void EnterState() = 0;

		virtual void Update() = 0;*/
	
	protected:
		// Is this really necessary
		// Where is the state id kept (if here, add it to constructor and getter)
		// StateID id;
	};

} // namespace mbe