#pragma once

///@file
///@brief Class mbe::AIState

#include <string>
#include <memory>

namespace mbe
{
	class AIState
	{
	public:
		typedef std::shared_ptr<AIState> Ptr;
		typedef std::weak_ptr<AIState> WPtr;
		typedef std::unique_ptr<AIState> UPtr;

		typedef std::string StateID;

	public:
		AIState();
		virtual ~AIState();

	public:
		virtual void EnterState() = 0;

		virtual void Update() = 0;
	
	protected:
		StateID nextState;
	};
} // namespace mbe