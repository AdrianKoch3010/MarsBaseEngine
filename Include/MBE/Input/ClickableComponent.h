#pragma once

/// @file
/// @brief Class mbe::ClickableComponent

#include <set>

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{

	class ClickableComponent : public Component
	{
	public:
		/// @brief Constructor
		/// @details By default both absorbeClick and bubbleUp is set to true
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		ClickableComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief  Default destructor
		virtual ~ClickableComponent() = default;

	public:
		inline void SetAbsorbeClick(bool value = true) { absorbeClick = value; }

		// if bubble up if set to true (in the Clickable System) the entity clicked event is raised for parent entities recursivly
		// Default is true
		inline void SetBubbleUp(bool value = true) { bubbleUp = value; }

		// Sets whether the entity clicked event also gets raised for all the parent entities
		inline const bool IsClickAbsorebd() const { return absorbeClick; }

		inline const bool DoesBubbleUp() const { return bubbleUp; }

		bool absorbeClick;
		bool bubbleUp;
	};

} // namespace mbe