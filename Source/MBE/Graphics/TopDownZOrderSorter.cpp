#include <MBE/Graphics/TopDownZOrderSorter.h>

using namespace mbe;

void TopDownZOrderSorter::operator()(std::vector<Entity::HandleID> & layer)
{
	// Loop through this list and assign the zOrder
	for (const auto & topDownObjectID : layer)
	{
		// Makes sure that the entity exists (This should be guaranteed by the entity manager)
		assert(Entity::GetObjectFromID(topDownObjectID) != nullptr && "TopDownZOrderSorter: The entity must exists");

		// Get the entity
		Entity & entity = *Entity::GetObjectFromID(topDownObjectID);

		// Make sure the entity has a RenderComponent
		/// Should this always be the case (yes when only use by the render system)
		assert(entity.HasComponent<RenderComponent>() && "TopDownZOrderSorter: The entity must have a mbe::RenderComponent");

		// Make sure the entity has a RenderInformationComponent (This should always be the case)
		assert(entity.HasComponent<RenderInformationComponent>() && "TopDownZOrderSorter: The entity must have a mbe::RenderInformationComponent");

		// Only sort entities with a mbe::TopDownInformationComponent
		if (entity.HasComponent<TopDownInformationComponent>())
		{
			// Assign the z-order based on the objects position, height and logicalBottomOffset
			const auto & topDownInformationComponent = entity.GetComponent<TopDownInformationComponent>();
			auto & renderComponent = entity.GetComponent<RenderComponent>();
			auto & renderInformationComponent = entity.GetComponent<RenderInformationComponent>();

			auto zOrder = renderComponent.GetGlobalBounds().top + renderComponent.GetGlobalBounds().height - topDownInformationComponent.GetLogicalBottomOffset();
			renderInformationComponent.SetZOrder(zOrder);
		}
	}
}
