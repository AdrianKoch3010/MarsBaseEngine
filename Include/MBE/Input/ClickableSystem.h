#pragma once

/// @file
/// @brief Class mbe::ClickableSystem

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityManager.h>
#include <MBE/Input/ClickableComponent.h>
#include <MBE/Input/MouseButtonReleasedEvent.h>
#include <MBE/Input/EntityClickedEvent.h>
#include <MBE/Graphics/RenderSystem.h>
#include <MBE/Graphics/RenderComponent.h>
#include <MBE/TransformComponent.h>

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