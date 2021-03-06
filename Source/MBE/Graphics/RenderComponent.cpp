#include <MBE/Graphics/RenderComponent.h>

using namespace mbe;


RenderComponent::RenderComponent(EventManager & eventManager, Entity & parentEntity)
	: Component(eventManager, parentEntity),
	hidden(false)
{
}

sf::FloatRect RenderComponent::GetGlobalBounds() const
{
	if (parentEntity.HasComponent<TransformComponent>())
		return parentEntity.GetComponent<TransformComponent>().GetWorldTransform().transformRect(this->GetLocalBounds());
	return this->GetLocalBounds();
}

bool RenderComponent::IsVisible(const sf::View & view) const
{
	// This algorithm does not account for view rotation

	// Create the view rectangle from the view's position and size
	sf::FloatRect viewRect;
	viewRect.left = view.getCenter().x - view.getSize().x / 2.f;
	viewRect.top = view.getCenter().y - view.getSize().y / 2.f;
	viewRect.width = view.getSize().x;
	viewRect.height = view.getSize().y;

	// Check whether it intersects with the global bounding box of this render component
	return viewRect.intersects(this->GetGlobalBounds());
}

void RenderComponent::SetHidden(bool value)
{
	hidden = value;
}

bool RenderComponent::IsHidden() const
{
	return hidden;
}
