#include <MBE/Graphics/RenderInformationComponent.h>

using namespace mbe;

using UnderlyingRenderLayer = std::underlying_type<RenderLayer>::type;

RenderInformationComponent::RenderInformationComponent(EventManager & eventManager, Entity & parentEntity, RenderLayer renderLayer, float zOrder) :
	Component(eventManager, parentEntity),
	renderLayer(renderLayer),
	zOrder(zOrder),
	// Set the default getter functions
	viewGetterFunction([](const Entity & entity) { return nullptr; }),
	windowGetterFunction([]() {return nullptr; })
{
}

const sf::View * RenderInformationComponent::GetView() const
{
	return viewGetterFunction(this->parentEntity);
}

const sf::RenderWindow * RenderInformationComponent::GetRenderWindow() const
{
	return windowGetterFunction();
}

void RenderInformationComponent::ResetViewGetterFunction()
{
	viewGetterFunction = [](const Entity & entity) {return nullptr; };
}

void RenderInformationComponent::ResetWindowGetterFunction()
{
	windowGetterFunction = []() {return nullptr; };
}

bool RenderInformationComponent::IsAboveOrEqual(const RenderInformationComponent & renderInformationComponent) const
{
	if (renderInformationComponent.GetRenderLayer() > this->GetRenderLayer())
		return true;
	else if (renderInformationComponent.GetRenderLayer() < this->GetRenderLayer())
		return false;
	
	// If the execution reaches this point the render layers must be equal
	return renderInformationComponent.GetZOrder() >= this->GetZOrder();
}

bool RenderInformationComponent::IsAboveOrEqual(const RenderInformationComponent & a, const RenderInformationComponent & b)
{
	return b.IsAboveOrEqual(a);
}

RenderLayer mbe::operator++(RenderLayer & renderLayer)
{
	return renderLayer = static_cast<RenderLayer>(static_cast<UnderlyingRenderLayer>(renderLayer) + 1);
}

bool mbe::operator==(RenderLayer a, RenderLayer b)
{
	return static_cast<UnderlyingRenderLayer>(a) == static_cast<UnderlyingRenderLayer>(b);
}

bool mbe::operator!=(RenderLayer a, RenderLayer b)
{
	return !(a == b);
}

bool mbe::operator>(RenderLayer a, RenderLayer b)
{
	return static_cast<UnderlyingRenderLayer>(a) > static_cast<UnderlyingRenderLayer>(b);
}

bool mbe::operator<(RenderLayer a, RenderLayer b)
{
	return static_cast<UnderlyingRenderLayer>(a) < static_cast<UnderlyingRenderLayer>(b);
}

bool mbe::operator>=(RenderLayer a, RenderLayer b)
{
	return !(a < b);
}

bool mbe::operator<=(RenderLayer a, RenderLayer b)
{
	return !(a > b);
}