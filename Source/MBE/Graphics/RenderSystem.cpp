#include <MBE/Core/ComponentValueChangedEvent.h>

#include <MBE/Graphics/RenderSystem.h>

using namespace mbe;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<TextureWrapperComponent>;

RenderSystem::RenderSystem(std::unique_ptr<sf::RenderWindow>& windowPtr, EventManager& eventManager, TextureWrapperHolder<>& textureWrapperHolder) :
	windowPtr(windowPtr),
	eventManager(eventManager),
	textureWrapperHolder(textureWrapperHolder)
{
	// Set the default views
	for (auto renderLayer = RenderLayer::Background; renderLayer != RenderLayer::LayerCount; ++renderLayer)
		viewDictionary[renderLayer] = windowPtr->getDefaultView();

	// Subscribe to the events
	std::function<void(const EntityCreatedEvent&)> onRenderEntityCreatedFunction = [this](const EntityCreatedEvent& event)
	{
		Entity::HandleID entityId = event.GetEntityID();
		AddRenderEntity(entityId);
	};

	std::function<void(const EntityRemovedEvent&)> onRenderEntityRemovedFunction = [this](const EntityRemovedEvent& event)
	{
		Entity::HandleID entityId = event.GetEntityID();
		RemoveRenderEntity(entityId);
	};

	std::function<void(const TextureWrapperChangedEvent&)> onTextureWrapperChangedFunction = [this](const TextureWrapperChangedEvent& event)
	{
		// Check that the texture wrapper has changed
		if (event.IsValueChanged("textureWrapper") == false)
			return;

		OnTextureWrapperComponentChangedEvent(event.GetComponent());
	};

	renderEntityCreatedSubscription = eventManager.Subscribe(onRenderEntityCreatedFunction);
	renderEntityRemovedSubscription = eventManager.Subscribe(onRenderEntityRemovedFunction);
	textureWrapperChangedSubscription = eventManager.Subscribe(onTextureWrapperChangedFunction);
}

RenderSystem::~RenderSystem()
{
	// Unsubscribe events
	eventManager.UnSubscribe<EntityCreatedEvent>(renderEntityCreatedSubscription);
	eventManager.UnSubscribe<EntityRemovedEvent>(renderEntityRemovedSubscription);
	eventManager.UnSubscribe<TextureWrapperChangedEvent>(textureWrapperChangedSubscription);

	// Make sure that all non-existing entity ids are deleted
	this->Refresh();

	// Reset the render information component getters
	for (auto renderLayer = RenderLayer::Background; renderLayer != RenderLayer::LayerCount; ++renderLayer)
	{
		const auto& renderEntityIdList = renderEntityDictionary[renderLayer];
		for (auto entityId : renderEntityIdList)
		{
			// This should always be the case
			assert(Entity::GetObjectFromID(entityId) != nullptr && "RenderSystem: The entity must exist");
			assert(Entity::GetObjectFromID(entityId)->HasComponent<RenderInformationComponent>() && "RenderSystem: The entity must have a mbe::RenderInformationComponent");

			// Get the render information component and reset the getters
			auto& renderInformationComponent = Entity::GetObjectFromID(entityId)->GetComponent<RenderInformationComponent>();
			renderInformationComponent.ResetViewGetterFunction();
			renderInformationComponent.ResetWindowGetterFunction();
		}
	}
}

void RenderSystem::Render()
{
	// Remove all expired nodes
	this->Refresh();

	for (auto renderLayer = RenderLayer::Background; renderLayer != RenderLayer::LayerCount; ++renderLayer)
	{
		// Draw all the render nodes in the current layer
		windowPtr->setView(viewDictionary[renderLayer]);
		auto& renderEntityIdList = renderEntityDictionary[renderLayer];

		// Assign the z-order based on the sorting method
		// Make sure a function has been registered
		if (zOrderAssignmentFunctionDictionary[renderLayer])
			zOrderAssignmentFunctionDictionary[renderLayer](renderEntityIdList);

		// Sort the render nodes based on their z-order
		SortByZOrder(renderEntityIdList);

		for (const auto renderEntityId : renderEntityIdList)
		{
			// Get the renderComponent of the entity that corresponds to the id
			const auto& renderComponent = Entity::GetObjectFromID(renderEntityId)->GetComponent<RenderComponent>();

			// Culling - only draw if the entity is visible on screen
			if (renderComponent.IsVisible(viewDictionary[renderLayer]) && renderComponent.IsHidden() == false)
			{
				// If the entity still exists
				// This should always be the case since expired entities are removed in the refresh function
				renderComponent.Draw(*windowPtr);
			}
		}
	}
}

void RenderSystem::SetZOrderAssignmentFunction(RenderLayer layer, ZOrderAssignmentFunction function)
{
	zOrderAssignmentFunctionDictionary[layer] = function;
}

void RenderSystem::SetView(RenderLayer renderLayer, const sf::View& view)
{
	viewDictionary[renderLayer] = view;
}

sf::View& RenderSystem::GetView(RenderLayer renderLayer)
{
	return viewDictionary[renderLayer];
}

const sf::View& RenderSystem::GetView(RenderLayer renderLayer) const
{
	return viewDictionary[renderLayer];
}

void RenderSystem::AddRenderEntity(Entity::HandleID entityId)
{
	auto* entity = Entity::GetObjectFromID(entityId);
	// Can't add a non existing node
	if (entity == nullptr)
		return;

	// The entity must have a mbe::RenderComponent and a mbe::RenderInformationComponent
	if (entity->HasComponent<RenderComponent>() == false
		|| entity->HasComponent<RenderInformationComponent>() == false)
		return;

	auto& renderInformationComponent = entity->GetComponent<RenderInformationComponent>();

	// Set the view and window getter functions
	renderInformationComponent.SetViewGetterFunction([this](const Entity& entity)
		{
			// Since this function can only be called within the render information component, the entity must also have that component
			return &this->GetView(entity.GetComponent<RenderInformationComponent>().GetRenderLayer());
		});
	renderInformationComponent.SetWindowGetterFunction([this]()
		{
			return &this->GetRenderWindow();
		});

	renderEntityDictionary[renderInformationComponent.GetRenderLayer()].push_back(entityId);
}

void RenderSystem::RemoveRenderEntity(Entity::HandleID entityId)
{
	// Get the renderNode (nullptr if if doesn't exist)
	auto* entity = Entity::GetObjectFromID(entityId);

	// If the pointed-to object no longer exists (or e.g. an invalid id has been passed)
	if (entity == nullptr)
		return;

	// The entity must have a mbe::RenderComponent and a mbe::RenderInformationComponent
	if (entity->HasComponent<RenderComponent>() == false
		&& entity->HasComponent<RenderInformationComponent>() == false)
		return;

	// Get the render layer;
	auto& renderInformationComponent = entity->GetComponent<RenderInformationComponent>();
	auto renderLayer = renderInformationComponent.GetRenderLayer();

	// Remove the node from this layer
	// std::find_if returns an it to the first intance for which the lambda returns true
	auto& renderEntityIdList = renderEntityDictionary[renderLayer];
	auto it = std::find_if(renderEntityIdList.begin(), renderEntityIdList.end(), [=](auto currentEntityId)
		{
			return entityId == currentEntityId;
		});

	if (it == renderEntityIdList.end())
		throw std::runtime_error("RenderSystem: The render entity could not be found");

	renderEntityIdList.erase(it);

	// Reset the RenderInformationComponent getters
	renderInformationComponent.ResetViewGetterFunction();
	renderInformationComponent.ResetWindowGetterFunction();
}

void RenderSystem::OnTextureWrapperComponentChangedEvent(TextureWrapperComponent& textureWrapperComponent)
{
	// Reassign all the texture wrappers
	using TextureID = TextureWrapperComponent::TextureID;
	for (TextureID textureId = 0; textureId <= textureWrapperComponent.GetHighestTextureID(); textureId++)
		textureWrapperComponent.SetTextureWrapper(&textureWrapperHolder[textureWrapperComponent.GetTextureWrapperID(textureId)], textureId);
}

void RenderSystem::Refresh()
{
	// Remove expired entities
	// Entities are automatically deleted when they expire (This is much more efficient than calling RemoveRenderEntity())
	for (auto& pair : renderEntityDictionary)
	{
		auto& entityList = pair.second;
		entityList.erase(std::remove_if(entityList.begin(), entityList.end(), [](auto currentEntityId)
			{
				Entity* entityPtr = Entity::GetObjectFromID(currentEntityId);
				// The second condition is not checked if the first one is true. So IsActive() is never called on nullptr
				return entityPtr == nullptr || entityPtr->IsActive() == false;
			}), entityList.end());
	}

	// Update the component render systems
	for (auto& componentRenderSystemPtr : componentRenderSystemList)
	{
		componentRenderSystemPtr->Update();
	}
}

bool RenderSystem::IsVisible(const Entity& entity, const sf::View& view)
{
	//const auto & boundingBox = entity.GetComponent<RenderComponent>().GetBoundingBox();

	return true;
}

void RenderSystem::SortByZOrder(std::vector<Entity::HandleID>& entityIdList)
{
	// Use insertion sort to sort by zOrder
	// Insertion sort is fast for nearly sorted lists
	// This will nearly always be the case since not many, if any object will changes between two frames
	// Furthermore, it is a consistent sort which prevents flickering if two render nodes have the z order
	size_t size = entityIdList.size();
	for (size_t i = 1; i < size; i++)
	{
		for (size_t j = i; j > 0 && Entity::GetObjectFromID(entityIdList[j - 1])->GetComponent<RenderInformationComponent>().GetZOrder() > Entity::GetObjectFromID(entityIdList[j])->GetComponent<RenderInformationComponent>().GetZOrder(); j--)
		{
			std::swap(entityIdList[j], entityIdList[j - 1]);
		}
	}
}