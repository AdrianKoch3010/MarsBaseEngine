//#include <MBE/Audio/SoundSystem.h>
//
//using namespace mbe;
//using mbe::event::EntityCreatedEvent;
//
//SoundSystem::SoundSystem(SoundManager & soundManager, EventManager & eventManager, const RenderSystem & renderSystem) :
//	soundManager(soundManager),
//	eventManager(eventManager),
//	renderSystem(renderSystem)
//{
//	// Subscribe to the entity created event
//	entityCreatedSubscription = eventManager.Subscribe(EventManager::TCallback<EntityCreatedEvent>([this](const EntityCreatedEvent & event) {
//		this->OnEntityCreated(event.GetEntityID());
//	}));
//}
//
//SoundSystem::~SoundSystem()
//{
//	eventManager.UnSubscribe<EntityCreatedEvent>(entityCreatedSubscription);
//}
//
//void SoundSystem::Update()
//{
//	this->Refresh();
//
//	for (const auto & pair : entitySoundDictionary)
//	{
//		const auto entityId = pair.first;
//		const auto & audioIdList = pair.second;
//		assert(Entity::GetObjectFromID(entityId) != nullptr && "SoundSystem: The entity must exist");
//		auto & entity = *Entity::GetObjectFromID(entityId);
//		const auto & audioComponent = entity.GetComponent<AudioComponent>();
//
//		sf::Vector2f position = this->CalculatePosition(entity);
//
//		// Update sounds
//		for (const auto & audioId : audioIdList)
//			soundManager.UpdateSound(audioId, audioComponent.GetAudioData(), position);
//	}
//}
//
//void SoundSystem::PlaySound(Entity::HandleID entityId, bool overrideSounds)
//{
//	auto it = entitySoundDictionary.find(entityId);
//	if (it == entitySoundDictionary.cend())
//		throw std::runtime_error("SoundSystem: No entity has been registered under this id: " + std::to_string(entityId));
//
//	auto & entity = *Entity::GetObjectFromID(entityId);
//	const auto & audioComponent = entity.GetComponent<AudioComponent>();
//	auto & audioIdList = it->second;
//
//	if (overrideSounds)
//	{
//		for (const auto & audioId : audioIdList)
//			soundManager.StopSound(audioId);
//	}
//
//	audioIdList.push_back(soundManager.PlaySound(audioComponent.GetAudioData(), this->CalculatePosition(entity)));
//}
//
//void SoundSystem::SetPaused(Entity::HandleID entityId, bool value)
//{
//	auto it = entitySoundDictionary.find(entityId);
//	if (it == entitySoundDictionary.cend())
//		throw std::runtime_error("SoundSystem: No entity has been registered under this id: " + std::to_string(entityId));
//
//	for (const auto & audioId : it->second)
//		soundManager.SetPaused(audioId, value);
//}
//
//void SoundSystem::StopSounds(Entity::HandleID entityId)
//{
//	auto it = entitySoundDictionary.find(entityId);
//	if (it == entitySoundDictionary.cend())
//		throw std::runtime_error("SoundSystem: No entity has been registered under this id: " + std::to_string(entityId));
//
//	for (const auto & audioId : it->second)
//		soundManager.StopSound(audioId);
//}
//
//void SoundSystem::OnEntityCreated(Entity::HandleID entityId)
//{
//	// The entity is guaranteed to exists
//	assert(Entity::GetObjectFromID(entityId) != nullptr && "SoundManager: The entity must exist");
//
//	// Add the entity to the entitySoundDictionary if it has the required component
//	if (Entity::GetObjectFromID(entityId)->HasComponent<AudioComponent>())
//		entitySoundDictionary.insert(std::make_pair(entityId, std::vector<SoundManager::ID>()));
//}
//
//void SoundSystem::Refresh()
//{
//	// Delete all deleted entities
//	for (auto it = entitySoundDictionary.cbegin(); it != entitySoundDictionary.cend(); )
//	{
//		if (Entity::GetObjectFromID(it->first) == nullptr)
//			it = entitySoundDictionary.erase(it);
//		else
//			++it;
//	}
//
//	// Remove stopped audioIds
//	for (auto & pair : entitySoundDictionary)
//	{
//		auto & audioIdList = pair.second;
//		for (auto it = audioIdList.cbegin(); it != audioIdList.cend(); )
//		{
//			const auto & audioId = *it;
//			if (soundManager.DoesExists(audioId) == false || soundManager.GetAudioStatus(audioId) == AudioData::AudioStatus::Stopped)
//				it = audioIdList.erase(it);
//			else
//				++it;
//		}
//	}
//}
//
//sf::Vector2f SoundSystem::CalculatePosition(const Entity & entity)
//{
//	sf::Vector2f position;
//	if (entity.HasComponent<AudioComponent>() && entity.HasComponent<TransformComponent>() && entity.HasComponent<RenderComponent>())
//	{
//		const auto & renderComponent = entity.GetComponent<RenderComponent>();
//		const auto & transformComponent = entity.GetComponent<TransformComponent>();
//
//		position = transformComponent.GetWorldPosition();
//		position = renderSystem.GetRenderWindow().mapPixelToCoords(static_cast<sf::Vector2i>(position),
//			renderSystem.GetRenderManager().GetView(renderComponent.GetRenderLayer()));
//	}
//	else if (entity.HasComponent<AudioComponent>() && entity.HasComponent<TransformComponent>())
//	{
//		const auto & transformComponent = entity.GetComponent<TransformComponent>();
//		position = transformComponent.GetWorldPosition();
//	}
//	else if (entity.HasComponent<AudioComponent>())
//	{
//		position = SoundManager::GetListenerPosition();
//	}
//
//	return position;
//}

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
	const auto & baseAudioEntities = entityManager.GetComponentGroup<BaseAudioComponent>();

	for (const auto entityId : baseAudioEntities)
	{
		// make sure the entity exists
		assert(Entity::GetObjectFromID(entityId) != nullptr && "AudioSystem: The entity must exists");

		const auto & entity = *Entity::GetObjectFromID(entityId);
		
		entity.GetComponent<BaseAudioComponent>().SetPosition(CalculatePosition(entity));
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
