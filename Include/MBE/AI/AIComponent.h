#pragma once
#pragma once

///@file
///@brief Class mbe::AIComponent

#include <memory>
#include <map>
#include <vector>
#include <algorithm>

#include <MBE/Core/Utility.h>
#include <MBE/Core/Component.h>
#include <MBE/AI/AIState.h>

namespace mbe
{
	// DEPRICATED
	// States should be light-weight objects that basically only contain references and methods
	// The can be created on the fly
	// Some other system (probably inheriting from AISystem will determine the transitions between states
	// The AIComponent manages a list of states that are currently active
	/// Maybe use string id instead of type --> makes it easier to switch to scripting / loading later on
	// States contain data on the action performed by the entity
	class AIComponent : public Component
	{
	public:
		typedef std::shared_ptr<AIComponent> Ptr;
		typedef std::weak_ptr<AIComponent> WPtr;
		typedef std::unique_ptr<AIComponent> UPtr;

		typedef typename AIState::StateID StateID;
		typedef std::map<StateID, typename AIState::UPtr> StateDictionary;

	public:
		AIComponent(EventManager & eventManager, Entity & parentEntity);
		~AIComponent() = default;

	public:
		// Create and add a new AIState
		template<class TState, typename ...TArguments>
		TState & AddState(StateID stateId, TArguments... arguments);

		// Throws if the state doesn't exist
		void RemoveState(StateID stateId);

		// Note that the ids are normalised
		std::vector<StateID> GetActiveStates() const;

		// Automatically normalises the id string
		bool IsStateActive(const StateID & id) const;

	private:
		StateDictionary stateDictionary;
	};

#pragma region Template Implementations

	template<class TState, typename ...TArguments>
	inline TState & AIComponent::AddState(StateID stateId, TArguments ...arguments)
	{
		NormaliseIDString(stateId);
		auto statePtr = std::make_unique<TState>(std::forward<TArguments>(arguments)...);
		stateDictionary.insert(std::make_pair(stateId, std::move(statePtr)));

		return *stateDictionary.at(stateId);
	}

#pragma endregion

} //namespace mbe