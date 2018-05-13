#include <MBE/Graphics/RenderInformationComponent.h>

using namespace mbe;

RenderInformationComponent::RenderInformationComponent(Entity & parentEntity, RenderLayer renderLayer, float zOrder) :
	Component(parentEntity),
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

RenderLayer mbe::operator++(RenderLayer & renderLayer)
{
	return renderLayer = static_cast<RenderLayer>(static_cast<std::underlying_type<RenderLayer>::type>(renderLayer) + 1);
}
