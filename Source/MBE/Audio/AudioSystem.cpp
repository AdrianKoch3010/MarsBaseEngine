#include <MBE/Audio/AudioSystem.h>

using namespace mbe;


// Calculate the audio position for entities with different components, similar to the clickable system
// Manage the life span of the sf::Sound / sf::Music instances?

AudioSystem::AudioSystem(const EntityManager & entityManager) :
	entityManager(entityManager)
{

}

void AudioSystem::Update()
{
	for (auto& entityId : entityManager.GetComponentGroup<BaseAudioComponent>())
	{
		// Update audio position
		entityId->GetComponent<BaseAudioComponent>().SetPosition(CalculatePosition(*entityId));

		// Delete audio entities that have finished playing
		if (entityId->GetComponent<BaseAudioComponent>().GetAudioStatus() == AudioData::AudioStatus::Stopped)
			entityId->Destroy();
	}
}

sf::Vector2f AudioSystem::CalculatePosition(const Entity & entity)
{
	sf::Vector2f position;
	if (entity.HasComponent<TransformComponent>() && entity.HasComponent<RenderInformationComponent>())
	{
		const auto & renderInformationComponent = entity.GetComponent<RenderInformationComponent>();
		const auto & transformComponent = entity.GetComponent<TransformComponent>();

		position = transformComponent.GetWorldPosition();

		// If the entity's view and render window is set, adjust for it
		if (renderInformationComponent.GetRenderWindow() != nullptr && renderInformationComponent.GetView() != nullptr)
		{
			position = renderInformationComponent.GetRenderWindow()->mapPixelToCoords(static_cast<sf::Vector2i>(position),
				*renderInformationComponent.GetView());
		}
	}
	else if (entity.HasComponent<TransformComponent>())
	{
		const auto & transformComponent = entity.GetComponent<TransformComponent>();
		position = transformComponent.GetWorldPosition();
	}
	else
	{
		position = AudioData::GetListenerPosition();
	}

	return position;
}
