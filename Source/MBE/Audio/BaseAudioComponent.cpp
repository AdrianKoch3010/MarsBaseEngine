#include <MBE/Audio/BaseAudioComponent.h>

using namespace mbe;

BaseAudioComponent::BaseAudioComponent(Entity & parentEntity) : Component(parentEntity)
{
	AudioData::IncrementNumberOfSounds();
}

BaseAudioComponent::~BaseAudioComponent()
{
	AudioData::DecrementNumberOfSounds();
}

sf::Vector2f BaseAudioComponent::CalculatePosition()
{
	sf::Vector2f position;
	if (parentEntity.HasComponent<TransformComponent>() && parentEntity.HasComponent<RenderInformationComponent>())
	{
		const auto & renderInformationComponent = parentEntity.GetComponent<RenderInformationComponent>();
		const auto & transformComponent = parentEntity.GetComponent<TransformComponent>();

		position = transformComponent.GetWorldPosition();

		// If the entity's view and render window is set, adjust for it
		if (renderInformationComponent.GetRenderWindow() != nullptr && renderInformationComponent.GetView() != nullptr)
		{
			position = renderInformationComponent.GetRenderWindow()->mapPixelToCoords(static_cast<sf::Vector2i>(position),
				*renderInformationComponent.GetView());
		}
	}
	else if (parentEntity.HasComponent<TransformComponent>())
	{
		const auto & transformComponent = parentEntity.GetComponent<TransformComponent>();
		position = transformComponent.GetWorldPosition();
	}
	else
	{
		position = AudioData::GetListenerPosition();
	}

	return position;
}