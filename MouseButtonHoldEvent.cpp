#include "MouseButtonHoldEvent.h"

using mbe::InputHandler;
using mbe::event::MouseButtonHoldEvent;

MouseButtonHoldEvent::MouseButtonHoldEvent(const InputHandler & inputHandler) : MouseButtonBaseEvent(inputHandler)
{
}

void MouseButtonHoldEvent::AddPressedButton(sf::Mouse::Button button)
{
	if (std::find(pressedButtonList.begin(), pressedButtonList.end(), button) != pressedButtonList.end())
		throw std::runtime_error("The same button cannot be added twice");
	
	pressedButtonList.push_back(button);
}

bool MouseButtonHoldEvent::IsButtonPressed(sf::Mouse::Button button) const
{
	return std::find(pressedButtonList.begin(), pressedButtonList.end(), button) != pressedButtonList.end();
}

std::ostream & mbe::event::operator<<(std::ostream & stream, const MouseButtonHoldEvent & event)
{
	stream << "MouseButtonHoldEvent:\tPosition: ";
	stream << event.GetClickPosition().x << ", " << event.GetClickPosition().y;
	stream << "\tButton: ";
	const auto & buttons = event.GetPressedButtonList();
	for (size_t i = 0; i < buttons.size(); i++)
	{
		const auto button = buttons[i];
		
		assert(mouseButtonStringDictionary.find(button) != mouseButtonStringDictionary.end() && "No string representation could be founf for this button");
		stream << mouseButtonStringDictionary.find(button)->second;

		stream << " (" << static_cast<unsigned int>(button) << ")";
		// Avoid the unnecessary new line
		if (i < buttons.size() - 1)
			stream << std::endl;
	}
	return stream;
}
