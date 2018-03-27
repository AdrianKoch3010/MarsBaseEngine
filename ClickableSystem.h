#pragma once

/// @file
/// @brief Class mbe::ClickableSystem

#include <SFML/System/Vector2.hpp>

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ClickableComponent.h"
#include "RenderSystem.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "MouseButtonReleasedEvent.h"
#include "EntityClickedEvent.h"

// There are 3 cases of managing audio clickable components
// - RenderComponent, TransformComponent, ClickableComponent
// --> Reverses view and entity transform
// - TransformComponent, ClickableComponent
// --> reverses entity transform
// - ClickableComponent
// --> Take the click position directly

namespace mbe
{

	class ClickableSystem
	{
	public:
		ClickableSystem(EventManager & eventManager, EntityManager & entityManager, RenderSystem & renderSystem);
		~ClickableSystem();

	private:
		void OnClick(sf::Vector2f clickPosition, sf::Mouse::Button button);

		// Reverses view and entity transfroms based on the entity's components
		sf::Vector2f CalculatePosition(const Entity & entity, sf::Vector2f clickPosition);

	private:
		EventManager & eventManager;
		EntityManager & entityManager;
		RenderSystem & renderSystem;

		EventManager::SubscriptionID onClickSubscription;
	};

} // namespace mbe