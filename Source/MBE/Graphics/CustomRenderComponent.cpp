#include <MBE/Graphics/CustomRenderComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(CustomRenderComponent, RenderComponent)

CustomRenderComponent::CustomRenderComponent(EventManager& eventManager, Entity& parentEntity) :
	RenderComponent(eventManager, parentEntity),
	localBounds(0.f, 0.f, 0.f, 0.f)
{
}

void CustomRenderComponent::Draw(sf::RenderTarget& target) const
{
	target.draw(vertices, renderStates);
}

sf::FloatRect CustomRenderComponent::GetLocalBounds() const
{
	return localBounds;
}


void CustomRenderComponent::SetLocalBounds(const sf::FloatRect& bounds)
{
	localBounds = bounds;
}

void CustomRenderComponent::SetRenderStates(const sf::RenderStates& states)
{
	renderStates = states;
}

const sf::RenderStates& CustomRenderComponent::GetRenderStates() const
{
	return renderStates;
}

sf::VertexArray& CustomRenderComponent::GetVertices()
{
	return vertices;
}

const sf::VertexArray& CustomRenderComponent::GetVertices() const
{
	return vertices;
}
