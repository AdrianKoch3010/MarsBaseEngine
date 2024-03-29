#pragma once

/// @file
/// @brief Class mbe::RenderSystem

#include <map>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <MBE/TransformComponent.h>
#include <MBE/Graphics/RenderComponent.h>
#include <MBE/Graphics/BaseComponentRenderSystem.h>
#include <MBE/Graphics/RenderInformationComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/Core/AssetHolder.h>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/EntityRemovedEvent.h>

using mbe::event::EntityCreatedEvent;
using mbe::event::EntityRemovedEvent;

namespace mbe
{
	/// @brief Takes care of drawing all entities with a mbe::RenderComponent in the correct order
	/// @details A mbe::Entity can be registered by raising the mbe::event::EntityCreatedEvent.
	/// Similarly, it can be unregistered by raising the mbe::event::RenderNodeRemovedEvent.
	/// @n The render system already has component renderers for the
	/// - SpriteRenderComponent
	/// - TiledRenderComponent
	/// @note Deleted entities will be removed automatically. Therefore, manually raising the
	/// mbe::event::EntityRemovedEvent is only necessary when the mbe::Entity should not be drawn but stay alive
	/// e.g. in order to add it again later.
	class RenderSystem
	{
	public:
		// The signature of the function that is used to assign the zOrder to a layer of render entities
		typedef std::function<void(std::vector<Entity::ID>&)> ZOrderAssignmentFunction;

		// When using the std::unordered_map operator[] the a default sf::View instance is created
		typedef std::unordered_map<RenderLayer, sf::View> ViewDictionary;

		// When using the std::unordered_map operator[] an emtpty std::vector<RenderObject::ID>() is created
		typedef std::unordered_map<RenderLayer, std::vector<Entity::ID>> RenderEntityDictionary;

		typedef std::unordered_map<RenderLayer, ZOrderAssignmentFunction> ZOrderAssignmentFunctionDictionary;

		typedef std::vector<BaseComponentRenderSystem::UPtr> ComponentRenderSystemList;

	public:
		/// @brief Constructor
		/// @param window A reference to the sf::RenderWindow that will be used to draw in
		/// @param eventManager A reference to the mbe::EventManager that will be used to listen out
		/// for a mbe::event::EntityCreatedEvent and mbe::event::RenderNodeRemovedEvent.
		/// @param textureWrapperHolder The texture wrapper holder that holds the textures indexed by the id in the mbe::TextureWrapperComponent
		RenderSystem(sf::RenderWindow& windowPtr, EventManager& eventManager, TextureWrapperHolder<>& textureWrapperHolder);

		/// @brief Default destructor
		~RenderSystem();

		/// @brief Draws all the registered entites
		/// @details Entities are only drawn when visible on screen.
		void Render();

		template <class TComponentRenderSystem, typename ...TArguments>
		void AddComponentRenderer(TArguments&&... arguments);

		void SetZOrderAssignmentFunction(RenderLayer layer, ZOrderAssignmentFunction function);

		void SetView(RenderLayer renderLayer, const sf::View& view);
		// This function should be prefered over SetView() since the existsing view can be modified
		sf::View& GetView(RenderLayer renderLayer);
		// Const overload
		const sf::View& GetView(RenderLayer renderLayer) const;

		inline sf::RenderWindow& GetRenderWindow() { return window; }
		inline const sf::RenderWindow& GetRenderWindow() const { return window; }

	private:
		void AddRenderEntity(Entity::ID entityId);
		void RemoveRenderEntity(Entity::ID entityId);

		// Sets the correct texture wrapper based on the context
		// Throws when the id has not been registerd
		void OnTextureWrapperComponentChangedEvent(TextureWrapperComponent& textureWrapperComponent);

		// Removes expires render entities
		void Refresh();

		// Culling
		bool IsVisible(const Entity& entity, const sf::View& view);

		// Performs an insertion sort
		// Assumes that render nodes exist for all render node ids
		static void SortByZOrder(std::vector<Entity::ID>& renderNodeIdList);

	private:
		// A reference to the pointer is stored so that when a new window is created the correct pointer is referenced
		sf::RenderWindow& window;
		TextureWrapperHolder<>& textureWrapperHolder;

		RenderEntityDictionary renderEntityDictionary;
		mutable ViewDictionary viewDictionary;
		ZOrderAssignmentFunctionDictionary zOrderAssignmentFunctionDictionary;
		ComponentRenderSystemList componentRenderSystemList;

		// Refernce to the event manager
		EventManager& eventManager;

		// SubscriptionId to unsubscribe the subscribed events
		EventManager::SubscriptionID renderEntityCreatedSubscription;
		EventManager::SubscriptionID renderEntityRemovedSubscription;
		EventManager::SubscriptionID textureWrapperChangedSubscription;
	};

#pragma region Template Implementations

	template<class TComponentRenderSystem, typename ...TArguments>
	inline void RenderSystem::AddComponentRenderer(TArguments&& ...arguments)
	{
		auto ptr = std::make_unique<TComponentRenderSystem>(std::forward<TArguments>(arguments)...);
		componentRenderSystemList.emplace_back(std::move(ptr));
	}

#pragma endregion

} // namespace mbe