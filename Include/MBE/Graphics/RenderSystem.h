#pragma once

/// @file
/// @brief The Class mbe::RenderSystem

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <functional>
#include <cassert>

#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/RenderLayer.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/EntityRemovedEvent.h>
#include <MBE/Core/EventManager.h>

using mbe::event::EntityCreatedEvent;
using mbe::event::EntityRemovedEvent;

namespace mbe
{
	/// @brief Takes care of drawing all entities with a mbe::RenderComponent in the correct order
	/// @details A mbe::Entity can be registered by raising the mbe::event::EntityCreatedEvent.
	/// Similarly, it can be unregistered by raising the mbe::event::RenderNodeRemovedEvent.
	/// @note Deleted entities will be removed automatically. Therefore, manually raising the
	/// mbe::event::EntityRemovedEvent is only necessary when the mbe::Entity should not be drawn but stay alive
	/// e.g. in order to add it again later.
	class RenderSystem
	{
	public:
		typedef std::function<void(std::vector<Entity::HandleID> &)> ZOrderAssignmentFunction;

	public:
		/// @brief Constructor
		/// @param window A reference to the sf::RenderWindow that will be used to draw in
		/// @param eventManager A reference to the mbe::EventManager that will be used to listen out
		/// for a mbe::event::EntityCreatedEvent and mbe::event::RenderNodeRemovedEvent.
		RenderSystem(sf::RenderWindow & window, EventManager & eventManager);

		/// @brief Default destructor
		~RenderSystem();

		/// @brief Draws all the registered entites
		/// @details Entities are only drawn when visible on screen.
		void Render();
		
		void SetZOrderAssignmentFunction(RenderComponent::RenderLayer layer, ZOrderAssignmentFunction function);

		void SetView(RenderComponent::RenderLayer renderLayer, const sf::View & view);
		// This function should be prefered over SetView() since the existsing view can be modified
		sf::View & GetView(RenderComponent::RenderLayer renderLayer);
		// Const overload
		const sf::View & GetView(RenderComponent::RenderLayer renderLayer) const;

		inline const sf::RenderWindow & GetRenderWindow() const { return window; }

	private:
		void AddRenderEntity(Entity::HandleID renderEntityId);
		void RemoveRenderEntity(Entity::HandleID renderEntityId);

		// Removes expires nodes
		void Refresh();

		// Culling
		bool IsVisible(const Entity & entity, const sf::View & view);

		// Performs an insertion sort
		// Assumes that render nodes exist for all render node ids
		static void SortByZOrder(std::vector<Entity::HandleID> & renderNodeIdList);

	private:
		sf::RenderWindow & window;

		std::array<detail::RenderLayer, RenderComponent::RenderLayer::LayerCount> renderLayers; // The index is important // will be default initialised

		std::array<ZOrderAssignmentFunction, RenderComponent::RenderLayer::LayerCount> zOrderAssignmentFunctions; // default initialised to an empty function

		// Refernce to the event manager
		EventManager & eventManager;

		// SubscriptionId to unsubscribe the subscribed events
		EventManager::SubscriptionID renderEntityCreatedSubscription;
		EventManager::SubscriptionID renderEntityRemovedSubscription;
	};

} // namespace mbe