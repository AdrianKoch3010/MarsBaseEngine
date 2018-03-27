#pragma once

/// @file
/// @brief Abstract class mbe::event::MouseButtonBaseEvent

#include <iostream>
#include <string>
#include <cassert>

#include <SFML/System/Vector2.hpp>

#include "Utility.h"

namespace mbe
{
	// Forward declare the mbe::InputHandler
	class InputHandler;

	namespace event
	{
		/// @brief Contains the click position and the mouse button
		class MouseButtonBaseEvent
			// make doxygen ignore the abstract definition
			/// @cond
			abstract
			/// @endcond
		{
		public:
			/// @brief Defines the type of an action that can be bound to a mouse event
			typedef std::string Action;

		public:
			/// @brief Constructor
			/// @details Initialises the click position with (0, 0)
			inline MouseButtonBaseEvent(const InputHandler & inputHandler) : inputHandler(inputHandler), clickPosition(0, 0) {}

			/// @brief Default destructor
			virtual ~MouseButtonBaseEvent() = default;

			/// @brief Sets the click position
			/// @param clickPosition The click position
			inline void SetClickPosition(sf::Vector2i clickPosition) { this->clickPosition = clickPosition; }

			/// @brief Sets the click position
			/// @param x The x coordinate of the click position
			/// @param y The y coordinate of the click position
			inline void SetClickPosition(int x, int y) { this->clickPosition.x = x; this->clickPosition.y = y; }

			/// @brief Gets the click position as a 2D-vector
			inline sf::Vector2i GetClickPosition() const { return clickPosition; }

			inline const InputHandler & GetInputHandler() const { return inputHandler; }

		private:
			sf::Vector2i clickPosition;

			const InputHandler & inputHandler;
		};

	} // namespace event
} // namespace mbe