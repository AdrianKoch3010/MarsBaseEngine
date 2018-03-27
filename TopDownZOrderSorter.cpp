#include "TopDownZOrderSorter.h"

using namespace mbe;

void TopDownZOrderSorter::operator()(std::vector<Entity::HandleID> & layer)
{
	// Loop through this list and assign the zOrder
	for (const auto & topDownObjectID : layer)
	{
		// Get the entity
		Entity * entity = Entity::GetObjectFromID(topDownObjectID);

		// Makes sure that the entity exists (This should be guaranteed by the entity manager)
		assert(entity != nullptr && "TopDownZOrderSorter: The entity must exists");

		// Make sure that the entity has a RenderComponent (This should always be the case)
		assert(entity->HasComponent<RenderComponent>() && "TopDownZOrderSorter: The entity must have a mbe::RenderComponent");

		// Only sort entities with a mbe::TopDownInformationComponent
		if (entity->HasComponent<TopDownInformationComponent>())
		{
			// Assign the z-order based on the objects position, height and logicalBottomOffset
			const auto & topDownInformationComponent = entity->GetComponent<TopDownInformationComponent>();
			auto & renderComponent = entity->GetComponent<RenderComponent>();

			auto zOrder = renderComponent.GetBoundingBox().top + renderComponent.GetBoundingBox().height - topDownInformationComponent.GetLogicalBottomOffset();
			renderComponent.SetZOrder(zOrder);
		}
	}
}
