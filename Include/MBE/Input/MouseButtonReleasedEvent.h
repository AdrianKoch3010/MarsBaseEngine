#pragma once

/// @file
/// @brief Final class mbe::event::MouseButtonReleasedEvent

#include <SFML/Window/Mouse.hpp>

#include <MBE/Input/MouseButtonBaseEvent.h>

namespace mbe
{
	namespace event
	{
		class MouseButtonReleasedEvent final : public MouseButtonBaseEvent
		{
		public:
			inline MouseButtonReleasedEvent(const InputHandler & inputHandler) : MouseButtonBaseEvent(inputHandler),
				button(sf::Mouse::Button::Left) {}
			~MouseButtonReleasedEvent() = default;

			inline void SetMouseButton(const sf::Mouse::Button button) { this->button = button; }
			inline sf::Mouse::Button GetButton() const { return button; }

			friend std::ostream & operator << (std::ostream & stream, const MouseButtonReleasedEvent & event);

		private:
			sf::Mouse::Button button;
		};

	} // namespace event
} // namespace mbe