#pragma once

/// @file
/// @brief Class mbe::event::EntityClickedEvent

#include <iostream>
#include <cassert>

#include <SFML/Window/Mouse.hpp>

#include <MBE/Graphics/RenderSystem.h>
#include <MBE/Core/Utility.h>

namespace mbe
{
	namespace event
	{

		class EntityClickedEvent
		{
		public:
			/// @brief Default constructor
			/// @details When using this constructor the entity id is set to null making sure that no entity exists under that id.
			/// The mouse button is set to sf::Mouse::Button::ButtonCount.
			/// @note When using the default constructor, the entity id and mouse button should be set before raising the event.
			/// This can be done using the mbe::EntityClickedEvent::SetEntityID() and mbe::EntityClickedEvent::SetMouseButton() methods respectively.
			EntityClickedEvent();

			/// @brief Constructor
			/// @param entityId The id of the entity that has been clicked
			/// @param button The sf::Mouse::Button with which the entity has been clicked 
			EntityClickedEvent(Entity::HandleID entityId, sf::Mouse::Button button);

			/// @brief Default destructor
			~EntityClickedEvent() = default;

		public:
			/// @brief Sets the entity id of the mbe::Entity that has been clicked
			/// @details This method is only needed when calling the default constructor.
			/// @param id The id of the entity that has been clicked
			inline void SetEntityID(Entity::HandleID id) { this->entityId = id; }

			/// @brief Sets the sf::Mouse::Button with which the entity has been clicked
			/// @details This method is only needed when calling the default constructor.
			/// @param button the sf::Mouse::Button that has been clicked
			inline void SetMouseButton(sf::Mouse::Button button) { this->mouseButton = button; }

			/// @brief Returns the id of the mbe::Entity that has been clicked
			inline Entity::HandleID GetEntityID() const { return entityId; }

			/// @brief Returns the sf::Mouse::Button with which the entity has been clicked
			inline sf::Mouse::Button GetMouseButton() const { return mouseButton; }

			/// @brief Allows this class to be written to an out stream
			/// @details This may be used to output the event's data to the console or a log file
			friend std::ostream& operator << (std::ostream& stream, const EntityClickedEvent& event);

		private:
			Entity::HandleID entityId;
			sf::Mouse::Button mouseButton;
		};

	} // namespace event
} // namespace mbe