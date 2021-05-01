#include <MBE/Graphics/TopDownZOrderSorter.h>

using namespace mbe;

void TopDownZOrderSorter::operator()(std::vector<Entity::ID>& layer)
{
	// Loop through this list and assign the zOrder
	for (auto& topDownEntityId : layer)
	{
		// Make sure the entity has a RenderComponent
		// Should this always be the case (yes when only used by the render system)
		const bool hasRequiredComponents = topDownEntityId->HasComponents<RenderComponent, RenderInformationComponent>();
		assert(hasRequiredComponents && "TopDownZOrderSorter: The entity must have a mbe::RenderComponent and mbe::RenderInformationComponent");

		// Only sort entities with a mbe::TopDownInformationComponent
		if (topDownEntityId->HasComponent<TopDownInformationComponent>())
		{
			// Assign the z-order based on the objects position, height and logicalBottomOffset
			const auto& topDownInformationComponent = topDownEntityId->GetComponent<TopDownInformationComponent>();
			auto& renderComponent = topDownEntityId->GetComponent<RenderComponent>();
			auto& renderInformationComponent = topDownEntityId->GetComponent<RenderInformationComponent>();

			auto zOrder = renderComponent.GetGlobalBounds().top + renderComponent.GetGlobalBounds().height - topDownInformationComponent.GetLogicalBottomOffset();
			renderInformationComponent.SetZOrder(zOrder);
		}
	}
}
