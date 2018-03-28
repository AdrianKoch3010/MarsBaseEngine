#include <MBE/Input/ActionEvents.h>


std::ostream & mbe::event::operator<<(std::ostream & stream, const mbe::event::ActionPressedEvent & event)
{
	stream << "ActionPressedEvent:\tAction: ";
	stream << event.GetAction();
	return stream;
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const mbe::event::ActionReleasedEvent & event)
{
	stream << "ActionReleasedEvent:\tAction: ";
	stream << event.GetAction();
	return stream;
}
