#pragma once

/// @file
/// @brief Final class mbe::event::MouseButtonHoldEvent

#include <vector>
#include <algorithm>

#include <SFML/Window/Mouse.hpp>

#include "MouseButtonBaseEvent.h"

namespace mbe
{
	namespace event
	{
		class MouseButtonHoldEvent final : public MouseButtonBaseEvent
		{
		public:
			MouseButtonHoldEvent(const InputHandler & inputHandler);
			~MouseButtonHoldEvent() = default;

		public:
			// Throws an exception if the same button is added more than once
			void AddPressedButton(sf::Mouse::Button button);
			bool IsButtonPressed(sf::Mouse::Button button) const;

			inline const std::vector<sf::Mouse::Button> & GetPressedButtonList() const { return pressedButtonList; }

			friend std::ostream & operator << (std::ostream & stream, const MouseButtonHoldEvent & event);

		private:
			std::vector<sf::Mouse::Button> pressedButtonList;
		};

	} // namespace event
} // namespace mbe