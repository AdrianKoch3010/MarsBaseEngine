#include "ActionHoldEvent.h"

using mbe::InputHandler;
using mbe::event::ActionHoldEvent;

ActionHoldEvent::ActionHoldEvent(const InputHandler & inputHandler) :
	inputHandler(inputHandler)
{
}

void ActionHoldEvent::AddActiveAction(Action action)
{
	if (std::find(activeActionList.begin(), activeActionList.end(), action) != activeActionList.end())
		throw std::runtime_error("The same button cannot be added twice");

	activeActionList.push_back(action);
}

bool ActionHoldEvent::IsActionActive(Action action) const
{
	return std::find(activeActionList.begin(), activeActionList.end(), action) != activeActionList.end();
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const ActionHoldEvent & event)
{
	stream << "ActionHoldEvent:\tActions: ";
	const auto & actionList = event.GetActiveActionList();
	for (size_t i = 0; i < actionList.size(); i++)
	{
		stream << "\t\t" << actionList[i];
		// Avoids the additional new line
		if (i < actionList.size() - 1)
			stream << std::endl;
	}
	return stream;
}
