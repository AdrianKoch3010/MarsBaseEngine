#pragma once

/// @file
/// @brief Class mbe::ClickableSystem

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityManager.h>
#include <MBE/Input/ClickableComponent.h>

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
		ClickableSystem(EventManager& eventManager, EntityManager& entityManager);
		~ClickableSystem();

	private:
		void OnClick(sf::Vector2f clickPosition, sf::Mouse::Button button);

		// Reverses view and entity transfroms based on the entity's components
		sf::Vector2f CalculatePosition(const Entity& entity, sf::Vector2f clickPosition);

		// Raise the mbe::event::EntityClickedEvent for the clicked entity and every entity connected to it
		void RaiseClickEvents(const ClickableComponent& clickableComponent, sf::Mouse::Button button);

	private:
		EventManager& eventManager;
		EntityManager& entityManager;

		EventManager::SubscriptionID onClickSubscription;
	};

} // namespace mbe