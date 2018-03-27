#include "StateManager.h"

using namespace mbe;

StateManager::StateManager(State::Context context) :
	context(context),
	stateStack(),
	pendingChangeQueue()
{
}

void StateManager::HandleSFMLEvents(const sf::Event & event)
{
	// Iterate from top to bottom, stop as soon as HandleSFMLEvents() returns false
	for (auto it = stateStack.end(); it != stateStack.begin(); )
	{
		--it;
		if (!(*it)->HandleSFMLEvents(event))
			break;
	}

	ApplyPendingChanges();
}

void StateManager::Update(sf::Time frameTime)
{
	// Iterate from top to bottom, stop as soon as Update() returns false
	for (auto it = stateStack.end(); it != stateStack.begin(); )
	{
		--it;
		if (!(*it)->Update(frameTime))
			break;
	}

	ApplyPendingChanges();
}

void StateManager::Render()
{
	// Iterate from bottom to top
	// This is necessary since lower states must be drawn first
	for (auto it = stateStack.begin(); it != stateStack.end(); ++it)
	{
		(*it)->Render();
	}

	ApplyPendingChanges();
}

void StateManager::PopState()
{
	pendingChangeQueue.push({ Action::Pop });
}

void StateManager::ClearStateStack()
{
	pendingChangeQueue.push({ Action::Clear });
}

void StateManager::ApplyPendingChanges()
{
	// As long as there are changes to be applied
	while (!pendingChangeQueue.empty())
	{
		// Keep popping from the queue
		
		switch (pendingChangeQueue.front().action)
		{
		case Action::Push:
		{
			/////////////DEBUG
			auto id = pendingChangeQueue.front().stateTypeId;

			// Find the factory function
			auto it = factoryFunctionDictionary.find(pendingChangeQueue.front().stateTypeId);

			// Make sure a factory function has been registered (this should always be the case)
			assert(it != factoryFunctionDictionary.end() && "No factory function has been registered for this id");

			// Call the factory function
			it->second(nullptr); // nullptr --> no data is required

			break;
		}
		case Action::PushWithData:
		{
			// Find the factory function
			auto it = factoryFunctionDictionary.find(pendingChangeQueue.front().stateTypeId);

			// Make sure a factory function has been registered (this should always be the case)
			assert(it != factoryFunctionDictionary.end() && "No factory function has been registered for this id");

			// Make sure that the data exists
			assert(dataToBePushed.empty() == false && "No data exists that could be pushed");

			// Get the data
			std::shared_ptr<void> data = dataToBePushed.front();

			// Call the factory function and pass in the data
			it->second(data);

			// Pop the data from the dataToBePushed Queue
			dataToBePushed.pop();
		}
			break;
		case Action::Pop:
			stateStack.pop_back();
			break;
		case Action::Clear:
			stateStack.clear();
			break;
		default:
			break;
		}
		// Pop after the changes have been applied
		pendingChangeQueue.pop();
	}
}