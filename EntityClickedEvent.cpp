#include "EntityClickedEvent.h"

using namespace mbe;
using namespace mbe::event;

EntityClickedEvent::EntityClickedEvent() :
	entityId(Entity::GetNullID()),
	mouseButton(sf::Mouse::Button::ButtonCount)
{
}

EntityClickedEvent::EntityClickedEvent(Entity::HandleID entityId, sf::Mouse::Button button) :
	entityId(entityId),
	mouseButton(button)
{
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const EntityClickedEvent & event)
{
	stream << "EnitityClickedEvent:\tEntityId: ";
	stream << event.GetEntityID();
	stream << "\tButton: ";

	assert(mouseButtonStringDictionary.find(event.GetMouseButton()) != mouseButtonStringDictionary.end() && "No string representation could be found for this button");
	stream << mouseButtonStringDictionary.find(event.GetMouseButton())->second;

	stream << " (" << static_cast<unsigned int>(event.GetMouseButton()) << ")";
	return stream;
}
