#pragma once

/// @file
/// @brief Class mbe::TopDownZOrderSystem

#include <vector>
#include <memory>
#include <cassert>

#include <MBE/Core/Entity.h>
#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/TopDownInformationComponent.h>

namespace mbe
{
	/// @brief Provides functionality to assign the correct z-order to top down objects in a layer
	class TopDownZOrderSorter
	{
	public:
		/// @brief Default constructor
		TopDownZOrderSorter() = default;

		/// @brief Default destructor
		~TopDownZOrderSorter() = default;

		/// @brief Assigns the z-order to all TopDownObjects in a layer based on their position and logical bottom offset
		/// @details This works best if all the entities in that layer are top down objects. The alroithm has complexity O(n)
		/// since the actual sorting is performed in the mbe::RenderSystem. The logical bottom offset is subtracted from the
		/// object's y-position. It can therefore be used to set the objects 'height' in the top down view/projection.
		/// @param layer The layer to sort the top down objects in
		/// @note In order to be sorted the the entity must be in the group: "TopDownObjects" and have the mbe::VisibleObjectInformationComponent
		void operator() (std::vector<Entity::HandleID> & layer);
	};
}