#include <MBE/Core/Utility.h>

#include <MBE/Input/ActionBaseEvent.h>

using mbe::InputHandler;
using mbe::event::ActionBaseEvent;

ActionBaseEvent::ActionBaseEvent(const InputHandler & inputHandler, Action action) :
	inputHandler(inputHandler),
	action(action)
{
}

bool ActionBaseEvent::IsActionActive(const Action& action) const
{
	return this->action == mbe::NormaliseIDString(action);
}
