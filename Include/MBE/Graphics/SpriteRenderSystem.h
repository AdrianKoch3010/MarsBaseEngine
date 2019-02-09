#pragma once

/// @file
/// @brief Class mbe::SpriteRenderSystem

#include <cassert>

#include <MBE/Graphics/BaseComponentRenderSystem.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityCreatedEvent.h>

#include <MBE/Graphics/SpriteRenderComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/TransformComponent.h>

namespace mbe
{

	class SpriteRenderSystem : public BaseComponentRenderSystem
	{
	public:
		typedef std::shared_ptr<SpriteRenderSystem> Ptr;
		typedef std::weak_ptr<SpriteRenderSystem> WPtr;
		typedef std::unique_ptr<SpriteRenderSystem> UPtr;

	public:
		SpriteRenderSystem(EventManager & eventManager, const EntityManager & entityManager);

		// Unsubscribe from events
		~SpriteRenderSystem();

	public:
		void Update() override;

	private:
		void OnEntityCreatedEvent(Entity & entity);

	private:
		EventManager & eventManager;
		EventManager::SubscriptionID entityCreatedSubscription;
	};

} // namespace mbe