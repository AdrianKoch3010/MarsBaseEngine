#pragma once

/// @file
/// @brief Class mbe::StateManager

#include <queue>
#include <memory>
#include <unordered_map>

#include <SFML/System/NonCopyable.hpp>

#include <MBE/Core/State.h>
#include <MBE/Core/AssetHolder.h>

namespace mbe
{
	namespace detail
	{
		/// @brief Defines the type of the component id
		/// @details This id is generated at runtime for each component class by calling mbe::detail::GetComponentTypeID()
		typedef std::size_t StateTypeID;

		/// @brief Returns a unique StateTypeID (for each function call)
		inline StateTypeID GetStateID() noexcept
		{
			static StateTypeID lastId = 0;
			return lastId++;
		}

		/// @brief Returns a StateTypeID for each type T
		/// @details Each component type will have its own unique id.
		/// The id will be the same for every instance of that type
		/// @tparam T The type for which the id is generated ie classes that inherit from mbe::State
		template <typename T>
		inline StateTypeID GetStateTypeID() noexcept
		{
			static StateTypeID typeId = GetStateID();
			return typeId;
		}

	} // namespace detail

	class StateManager : private sf::NonCopyable
	{
	public:
		/// @brief allows for writing StateManager::Ptr instead of std::unique_ptr<StateManager>
		typedef std::unique_ptr<StateManager> Ptr;

	private:
		/// @brief Defines the type of function used to create a new state
		typedef std::function<void(std::shared_ptr<void>)> FactoryFunction;

	private:
		/// @brief The possible actions that can be performed on the state
		enum class Action
		{
			/// @brief Create a new mbe::State and push it onto the state stack
			Push,
			/// @brief Create a new mbe::State that takes an additional data argument and push it onto the state stack
			PushWithData,
			/// @brief Pop the topmost state from the stack
			Pop,
			/// @brief Clear the entire state stack
			Clear
		};

	public:
		/// @brief Constructor
		/// @param context The context that will be passed to every state
		explicit StateManager(State::Context& context);

		/// @brief Default destructor
		~StateManager() = default;

		/// @brief Calls the HandleSFMLEvents() methods of the states currently in the state stack
		/// @details The top-most states's method is called first. The HandlerSFMLEvents() methods
		/// of the states below are called as long as the HandleSFMLEvents() method of the state above returns true.
		/// Therefore, returning false in the state's HandleSFMLEvents() method blocks the events from reaching states below it on the stack.
		/// @param event The SFML event containing the event data
		void HandleSFMLEvents(const sf::Event & event);

		/// @brief Calls the Update() methods of the states currently in the state stack
		/// @details The top-most states's method is called first. The Update() methods
		/// of the states below are called as long as the Update() method of the state above returns true.
		/// Therefore, returning false in the state's Update() method blocks the states below from being updated.
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime);

		/// @brief Calls the Render() methods of the states currently on the state stack
		/// @details The top-most states's method is called first. The Render() methods
		/// of the states below are called as long as the Render() method of the state above returns true.
		/// Therefore, returning false in the state's Render() method blocks the states below from being rendered.
		void Render();

		/// @brief Creates a new state and pushes it onto the state stack
		/// @details This function is for pushing states that take an additional argument of type std::shared_ptr<TData>.
		/// For states that take no additional data in their constructor use the other overload of the PushState() method.
		/// @tparam TState The type of State to be created. Ususally inherited from mbe::State. It's contructor must take
		/// a reference to this mbe::StateManager and an instance of the mbe::State::Context class as well as a third argument
		/// of type std::shared_ptr<TData>
		/// @tparam TData The type of the data passed to the state's constructor. This must be equivalent with the type expected
		/// by the contructor of TState.
		/// @param data The data passed to the state. All data must be contained in a std::shared_ptr.
		/// @note Using one of the PushState() method overloads is the only way to create a new state.
		template <class TState, class TData>
		void PushState(std::shared_ptr<TData> data);

		/// @brief Creates a new state and pushes it onto the state stack
		/// @details This function is for pushing states that don't take an additional argument.
		/// For states that take an additional argument in their constructor use the other overload of the PushState() method.
		/// @tparam TState The type of State to be created. Ususally inherited from mbe::State. It's contructor must take
		/// a reference to this mbe::StateManager and an instance of the mbe::State::Context class.
		/// @note Using one of the PushState() method overloads is the only way to create a new state.
		template <class TState>
		void PushState();

		/// @brief Pops the top-most state from the state stack
		/// @details The state will automatically be destroyed.
		void PopState();

		/// @brief Removes all states from the state stack
		/// @details The states will automatically be destroyed
		void ClearStateStack();

		/// @brief Returns whether the state stack is empty
		/// @details This can be used e.g. as a condition to close the game.
		inline bool IsStateStackEmpty() const { return stateStack.empty(); }

	private:
		void ApplyPendingChanges();

	#pragma region Pending Change
	private:
		struct PendingChange
		{
			inline PendingChange(Action action, detail::StateTypeID stateTypeId = 0) : action(action), stateTypeId(stateTypeId) {}

			Action action;
			detail::StateTypeID stateTypeId;
		};
	#pragma endregion

	private:
		// use a std::stack for the state stack
		std::vector<State::Ptr> stateStack;

		std::queue<PendingChange> pendingChangeQueue;
		std::queue<std::shared_ptr<void>> dataToBePushed;
		std::unordered_map<detail::StateTypeID, FactoryFunction> factoryFunctionDictionary;

		State::Context& context;
	};

#pragma region Template Implementations

	template<class TState, class TData>
	inline void StateManager::PushState(std::shared_ptr<TData> data)
	{
		// Get the state's type id
		detail::StateTypeID typeId = detail::GetStateTypeID<TState>();

		// Push the action onto the action queue
		pendingChangeQueue.push({ Action::PushWithData, typeId });

		// Push the data onto the data queue
		// Automatically cast to std::shared_ptr<void>
		dataToBePushed.push(data);

		// Return if this state has already been registered
		if (factoryFunctionDictionary.find(typeId) != factoryFunctionDictionary.end())
			return;

		// Register the factory function for the state if it has not been registered before
		factoryFunctionDictionary[typeId] = [this](std::shared_ptr<void> data)
		{
			State::Ptr state = std::make_unique<TState>(*this, context, std::static_pointer_cast<TData>(data));
			stateStack.push_back(std::move(state));
		};
	}

	template<class TState>
	inline void StateManager::PushState()
	{
		// Get the state's type id
		detail::StateTypeID typeId = detail::GetStateTypeID<TState>();

		pendingChangeQueue.push({ Action::Push, typeId });

		// Return if this state has already been registered
		if (factoryFunctionDictionary.find(typeId) != factoryFunctionDictionary.end())
			return;

		// Register the factory function for the state if it has not been registered before
		factoryFunctionDictionary[typeId] = [this](std::shared_ptr<void> data)
		{
			State::Ptr state = std::make_unique<TState>(*this, context);
			stateStack.push_back(std::move(state));
		};
	}

#pragma endregion

} // namespace mbe