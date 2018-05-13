//#pragma once
//
///// @file
///// @brief Class mbe::SoundSystem
//
//#include <vector>
//#include <unordered_map>
//#include <cassert>
//
//#include <MBE/Core/EventManager.h>
//#include <MBE/Core/EntityCreatedEvent.h>
//#include <MBE/Core/Entity.h>
//#include <MBE/Graphics/RenderComponent.h>
//#include <MBE/Graphics/RenderSystem.h>
//#include <MBE/Audio/AudioComponent.h>
//#include <MBE/Audio/SoundManager.h>
//#include <MBE/TransformComponent.h>
//
//
//// There are 3 cases of managing audio components
//// - RenderComponent, TransformComponent, AudioComponent
//// --> Sets the audio position relative to the transform component and dependent on the view of the renderComponent
//// - TransformComponent, AudioComponent
//// --> Sets the audio position relative to the transform component
//// - AudioComponent
//// --> Plays non-spacialised audio
//
//namespace mbe
//{
//
//	class SoundSystem
//	{
//	public:
//		/// @brief Constructor
//		SoundSystem(SoundManager & soundManager, EventManager & eventManager, const RenderSystem & renderSystem);
//
//		/// @Destructor
//		~SoundSystem();
//
//	public:
//		// Should be called once a frame
//		// Refreshes the managed audio components data
//		void Update();
//
//		// Playes the sound with the parameters described in the entity's audio component / transform component / renderComponent
//		// If overrideSounds is set to true, all playing sounds are stopped and only the current sound is played
//		void PlaySound(Entity::HandleID entityId, bool overrideSounds = false);
//
//		// Pauses / unpauses all sound and music instances associated with this entity
//		void SetPaused(Entity::HandleID entityId, bool value = true);
//
//		// Stops all sound and music instances associated with this entity
//		void StopSounds(Entity::HandleID entityId);
//
//	private:
//		void OnEntityCreated(Entity::HandleID entityId);
//		
//		void Refresh();
//
//		sf::Vector2f CalculatePosition(const Entity & entity);
//
//	private:
//		SoundManager & soundManager;
//		EventManager & eventManager;
//		const RenderSystem & renderSystem;
//
//		std::unordered_map<Entity::HandleID, std::vector<SoundManager::ID>> entitySoundDictionary;
//
//		EventManager::SubscriptionID entityCreatedSubscription;
//	};
//	
//} // namespace mbe