#include "RenderSystem.h"

using namespace mbe;

RenderSystem::RenderSystem(sf::RenderWindow & window, EventManager & eventManager) :
	window(window),
	eventManager(eventManager)
{
	// Set the default views
	for (auto & renderLayer : renderLayers)
	{
		renderLayer.SetView(window.getDefaultView());
	}

	// Subscribe the events
	std::function<void(const EntityCreatedEvent &)> onRenderEntityCreatedFunction = [this](const EntityCreatedEvent & event)
	{
		Entity::HandleID entityId = event.GetEntityID();
		AddRenderEntity(entityId);
	};

	std::function<void(const EntityRemovedEvent &)> onRenderEntityRemovedFunction = [this](const EntityRemovedEvent & event)
	{
		Entity::HandleID entityId = event.GetEntityID();
		RemoveRenderEntity(entityId);
	};

	renderEntityCreatedSubscription = eventManager.Subscribe(onRenderEntityCreatedFunction);
	renderEntityRemovedSubscription = eventManager.Subscribe(onRenderEntityRemovedFunction);
}

mbe::RenderSystem::~RenderSystem()
{
	eventManager.UnSubscribe<EntityCreatedEvent>(renderEntityCreatedSubscription);
	eventManager.UnSubscribe<EntityRemovedEvent>(renderEntityRemovedSubscription);
}

void RenderSystem::Render()
{
	// Remove all expired nodes
	this->Refresh();
	
	for (size_t i = 0; i < RenderComponent::RenderLayer::LayerCount; i++)
	{
		// Draw all the render nodes in the current layer
		window.setView(renderLayers[i].GetView());
		std::vector<Entity::HandleID> & renderEntityIdList = renderLayers[i].GetRenderEntityIdList();

		// Assign the z-order based on the sorting method
		// Make sure a function has been registered
		if (zOrderAssignmentFunctions[i])
			zOrderAssignmentFunctions[i](renderEntityIdList);
		
		// Sort the render nodes based on their z-order
		SortByZOrder(renderEntityIdList);

		for (const auto renderEntityId : renderEntityIdList)
		{
			// Get the pointer that corresponds to the id
			auto * entity = Entity::GetObjectFromID(renderEntityId);

			// Culling - only draw if the entity is visible on screen
			if (IsVisible(*entity, renderLayers[i].GetView()))
			{
				// If the entity still exists
				// This should always be the case since expired entities are removed in the refresh function
				entity->GetComponent<RenderComponent>().Draw(window);
			}
		}
	}
}

void RenderSystem::SetZOrderAssignmentFunction(RenderComponent::RenderLayer layer, ZOrderAssignmentFunction function)
{
	zOrderAssignmentFunctions[layer] = function;
}

void RenderSystem::SetView(RenderComponent::RenderLayer renderLayer, const sf::View & view)
{
	renderLayers[renderLayer].SetView(view);
}

sf::View & mbe::RenderSystem::GetView(RenderComponent::RenderLayer renderLayer)
{
	return renderLayers[renderLayer].GetView();
}

const sf::View & mbe::RenderSystem::GetView(RenderComponent::RenderLayer renderLayer) const
{
	return renderLayers[renderLayer].GetView();
}

void RenderSystem::AddRenderEntity(Entity::HandleID renderEntityId)
{
	auto * entity = Entity::GetObjectFromID(renderEntityId);
	// Can't add a non existing node
	if (entity == nullptr)
		return;

	// The entity must have a mbe::RenderComponent
	if (entity->HasComponent<RenderComponent>() == false)
		return;

	auto layer = entity->GetComponent<RenderComponent>().GetRenderLayer();
	renderLayers[layer].AddRenderEntity(renderEntityId);
}

void RenderSystem::RemoveRenderEntity(Entity::HandleID renderEntityId)
{
	// Get the renderNode (nullptr if if doesn't exist)
	auto * entity = Entity::GetObjectFromID(renderEntityId);

	// If the pointed-to object no longer exists (or e.g. an invalid id has been passed)
	if (entity == nullptr)
		return;

	// The entity must have a mbe::RenderComponent
	if (entity->HasComponent<RenderComponent>() == false)
		return;

	// Get the render layer;
	auto renderLayer = entity->GetComponent<RenderComponent>().GetRenderLayer();
	
	// Remove the node from this layer
	renderLayers[renderLayer].RemoveRenderEntity(renderEntityId);
}

void RenderSystem::Refresh()
{
	// Remove expired entities
	// Entities are automatically deleted when they expire (This is much more efficient than calling RemoveRenderEntity())
	for (auto & renderLayer : renderLayers)
	{
		auto & entityList = renderLayer.GetRenderEntityIdList();
		entityList.erase(std::remove_if(entityList.begin(), entityList.end(), [](auto currentEntityId)
		{
			return Entity::GetObjectFromID(currentEntityId) == nullptr;
		}), entityList.end());
	}
}

bool RenderSystem::IsVisible(const Entity & entity, const sf::View & view)
{
	const auto & boundingBox = entity.GetComponent<RenderComponent>().GetBoundingBox();

	return true;
}

void RenderSystem::SortByZOrder(std::vector<Entity::HandleID> & entityIdList)
{
	// Use insertion sort to sort by zOrder
	// Insertion sort is fast for nearly sorted lists
	// This will nearly always be the case since not many, if any object will changes between two frames
	// Furthermore, it is a consistent sort which prevents flickering if two render nodes have the z order
	size_t size = entityIdList.size();
	for (size_t i = 1; i < size; i++)
	{
		for (size_t j = i; j > 0 && Entity::GetObjectFromID(entityIdList[j - 1])->GetComponent<RenderComponent>().GetZOrder() > Entity::GetObjectFromID(entityIdList[j])->GetComponent<RenderComponent>().GetZOrder(); j--)
		{
			std::swap(entityIdList[j], entityIdList[j - 1]);
		}
	}
}
