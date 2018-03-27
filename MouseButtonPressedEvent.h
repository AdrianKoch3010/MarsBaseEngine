#pragma once

/// @file
/// @brief Final class mbe::event::MouseButtonPressedEvent

#include <SFML/Window/Mouse.hpp>

#include "MouseButtonBaseEvent.h"

namespace mbe
{
	namespace event
	{
		class MouseButtonPressedEvent final : public MouseButtonBaseEvent
		{
		public:
			inline MouseButtonPressedEvent(const InputHandler & inputHandler) : MouseButtonBaseEvent(inputHandler),
				button(sf::Mouse::Button::Left) {}
			~MouseButtonPressedEvent() = default;

			inline void SetMouseButton(sf::Mouse::Button button) { this->button = button; }
			inline sf::Mouse::Button GetButton() const { return button; }

			friend std::ostream & operator << (std::ostream & stream, const MouseButtonPressedEvent & event);

		private:
			sf::Mouse::Button button;
		};

	} // namespace event
} // namespace mbe