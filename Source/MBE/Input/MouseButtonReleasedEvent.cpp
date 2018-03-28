#include <MBE/Input/MouseButtonReleasedEvent.h>

std::ostream & mbe::event::operator<<(std::ostream & stream, const MouseButtonReleasedEvent & event)
{
	stream << "MouseButtonReleasedEvent:\tPosition: ";
	stream << event.GetClickPosition().x << ", " << event.GetClickPosition().y;
	stream << "\tButton: ";
	
	assert(mouseButtonStringDictionary.find(event.GetButton()) != mouseButtonStringDictionary.end() && "No string representation could be founf for this button");
	stream << mouseButtonStringDictionary.find(event.GetButton())->second;

	stream << " (" << static_cast<unsigned int>(event.GetButton()) << ")";
	return stream;
}
