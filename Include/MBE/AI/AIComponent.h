#pragma once
#pragma once

///@file
///@brief Class mbe::AIComponent

#include <memory>
#include <map>
#include <vector>

#include <MBE/Core/Component.h>
#include <MBE/AI/AIState.h>

namespace mbe
{
	// States should be light-weight objects that basically only contain references and methods
	// The can be created on the fly
	// Some other system (probably inheriting from AISystem will determine the transitions between states
	// The AIComponent manages a list of states that are currently active
	/// Maybe use string id instead of type --> makes it easier to switch to scripting / loading later on
	class AIComponent : public Component
	{
	public:
		typedef std::shared_ptr<AIComponent> Ptr;
		typedef std::weak_ptr<AIComponent> WPtr;
		typedef std::unique_ptr<AIComponent> UPtr;

		typedef size_t StateID;
		typedef std::map<StateID, typename AIState::UPtr> StateDictionary;

	public:
		AIComponent(EventManager & eventManager, Entity & parentEntity);
		~AIComponent() = default;

	public:
		// This method should be moved to an appropreate system
		void Update(sf::Time frameTime) override;

		// Create and add a new AIState
		template<class TState, typename ...TArguments>
		TState & AddState(TArguments... arguments);

		// Throws if the state doesn't exist
		template<class TState>
		void RemoveState();

		std::vector<StateID> GetActiveStates() const;

		bool IsStateActive(const StateID & id) const;

	private:
		StateDictionary stateDictionary;
	};

#pragma region Template Implementations



#pragma endregion

	template<class TState, typename ...TArguments>
	inline TState & AIComponent::AddState(TArguments ...arguments)
	{
		// TODO: hier Rückgabeanweisung eingeben
	}
	template<class TState>
	inline void AIComponent::RemoveState()
	{
	}
} //namespace mbe