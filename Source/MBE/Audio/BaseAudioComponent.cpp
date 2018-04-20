#include <MBE/Audio/BaseAudioComponent.h>

using namespace mbe;

float BaseAudioComponent::listenerZ = 300.f;

unsigned short int BaseAudioComponent::totalNumberOfSounds = 0u;
const unsigned short int BaseAudioComponent::maxNumberOfSounds = 255u; // Internal limit set by SFML / OpenAL

BaseAudioComponent::BaseAudioComponent(Entity & parentEntity) : Component(parentEntity)
{
}

sf::Vector2f mbe::BaseAudioComponent::CalculatePosition()
{
	sf::Vector2f position;
	if (parentEntity.HasComponent<TransformComponent>() && parentEntity.HasComponent<RenderComponent>())
	{
		const auto & renderComponent = parentEntity.GetComponent<RenderComponent>();
		const auto & transformComponent = parentEntity.GetComponent<TransformComponent>();

		position = transformComponent.GetWorldPosition();
		position = renderSystem.GetRenderWindow().mapPixelToCoords(static_cast<sf::Vector2i>(position),
			renderSystem.GetRenderManager().GetView(renderComponent.GetRenderLayer()));
	}
	else if (parentEntity.HasComponent<TransformComponent>())
	{
		const auto & transformComponent = parentEntity.GetComponent<TransformComponent>();
		position = transformComponent.GetWorldPosition();
	}
	else
	{
		position = GetListenerPosition();
	}

	return position;
}

void BaseAudioComponent::SetListenerPosition(const sf::Vector2f & position)
{
	sf::Listener::setPosition(position.x, -position.y, listenerZ);
}

sf::Vector2f BaseAudioComponent::GetListenerPosition()
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

float BaseAudioComponent::Convert2DTo3DMinDistance(float minDistance2d)
{
	// Pythagoras
	return std::sqrt(std::pow(minDistance2d, 2) + std::pow(listenerZ, 2));
}

float BaseAudioComponent::Convert3DTo2DMinDistance(float minDistance3d)
{
	// Inverse function of Convert2DTo3DMinDistance
	return std::sqrt(std::pow(minDistance3d, 2) - std::pow(listenerZ, 2));
}
