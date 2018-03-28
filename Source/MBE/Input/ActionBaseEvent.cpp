#include <MBE/Input/ActionBaseEvent.h>

using mbe::InputHandler;
using mbe::event::ActionBaseEvent;

ActionBaseEvent::ActionBaseEvent(const InputHandler & inputHandler, Action action) :
	inputHandler(inputHandler),
	action(action)
{
}