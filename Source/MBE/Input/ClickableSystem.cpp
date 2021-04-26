#include <MBE/Input/ClickableSystem.h>

#include <MBE/Input/MouseButtonReleasedEvent.h>
#include <MBE/Input/EntityClickedEvent.h>
#include <MBE/Graphics/RenderInformationComponent.h>
#include <MBE/TransformComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>

using namespace mbe;
using mbe::event::MouseButtonReleasedEvent;

ClickableSystem::ClickableSystem(EventManager& eventManager, EntityManager& entityManager) :
	eventManager(eventManager),
	entityManager(entityManager)
{
	onClickSubscription = eventManager.Subscribe(EventManager::TCallback<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent& event)
		{
			this->OnClick(static_cast<sf::Vector2f>(event.GetClickPosition()), event.GetButton());
		}));
}

ClickableSystem::~ClickableSystem()
{
	eventManager.UnSubscribe<MouseButtonReleasedEvent>(onClickSubscription);
}

void ClickableSystem::OnClick(sf::Vector2f clickPosition, sf::Mouse::Button button)
{
	// Ptr since entity exist
	std::vector<const Entity*> clickedEntityList;

	for (const auto& entityId : entityManager.GetComponentGroup<ClickableComponent>())
	{
		// The entity must have an mbe::TextureWrapperComponent
		if (entityId->HasComponent<TextureWrapperComponent>() == false)
			continue;

		const auto& clickableComponent = entityId->GetComponent<ClickableComponent>();
		const auto& textureWrapperComponent = entityId->GetComponent<TextureWrapperComponent>();
		const auto& textureRect = textureWrapperComponent.GetTextureRect();
		const auto pixelMaskPtr = textureWrapperComponent.GetTextureWrapper().GetPixelMask();

		// The pixel mask must be created
		if (pixelMaskPtr == nullptr)
			continue;

		if (pixelMaskPtr->Contains(CalculatePosition(*entityId, clickPosition), textureRect))
			//if (clickableComponent.Contains(CalculatePosition(entity, clickPosition)))
		{
			// If the entity has a renderInformationComponent the 'drawing' order + clickAbsorbtion must be taken into account
			if (entityId->HasComponent<RenderInformationComponent>())
				clickedEntityList.push_back(&*entityId);
			else
				RaiseClickEvents(clickableComponent, button);
		}
	}

	// Sort the clicked entity list by render order (in decending order)
	std::sort(clickedEntityList.begin(), clickedEntityList.end(), [](const Entity* a, const Entity* b) {
		return b->GetComponent<RenderInformationComponent>().IsAbove(a->GetComponent<RenderInformationComponent>());
		});


	// Loop through all clicked entities and figure out for which the event must be raised (depending on which one is drawn on top of each other)
	for (const auto* const entityPtr : clickedEntityList)
	{
		const auto& clickableComponent = entityPtr->GetComponent<ClickableComponent>();

		RaiseClickEvents(clickableComponent, button);

		if (clickableComponent.IsClickAbsorebd())
			return;
	}
}

sf::Vector2f ClickableSystem::CalculatePosition(const Entity& entity, sf::Vector2f clickPosition)
{
	if (entity.HasComponent<ClickableComponent>() && entity.HasComponent<TransformComponent>() && entity.HasComponent<RenderInformationComponent>())
	{
		const auto& transformComponent = entity.GetComponent<TransformComponent>();
		const auto& renderInformationComponent = entity.GetComponent<RenderInformationComponent>();
		const auto& view = *renderInformationComponent.GetView();

		// Reverse the view transform
		clickPosition = renderInformationComponent.GetRenderWindow()->mapPixelToCoords(static_cast<sf::Vector2i>(clickPosition), view);
		// Reverse the entity transform
		clickPosition = transformComponent.GetWorldTransform().getInverse().transformPoint(clickPosition);
	}
	else if (entity.HasComponent<ClickableComponent>() && entity.HasComponent<TransformComponent>())
	{
		const auto& transformComponent = entity.GetComponent<TransformComponent>();

		// Reverse the entity transform
		clickPosition = transformComponent.GetWorldTransform().getInverse().transformPoint(clickPosition);
	}

	return clickPosition;
}

void ClickableSystem::RaiseClickEvents(const ClickableComponent& clickableComponent, sf::Mouse::Button button)
{
	if (clickableComponent.DoesBubbleUp() == false)
	{
		event::EntityClickedEvent entityClickedEvent;
		entityClickedEvent.SetEntityID(clickableComponent.GetParentEntity().GetHandleID());
		entityClickedEvent.SetMouseButton(button);
		eventManager.RaiseEvent(entityClickedEvent);
		return;
	}

	for (auto entityId = clickableComponent.GetParentEntity().GetHandleID(); entityId.Valid(); )
	{
		const auto& clickedEntity = *Entity::GetObjectFromID(entityId);

		event::EntityClickedEvent entityClickedEvent;
		entityClickedEvent.SetEntityID(clickedEntity.GetHandleID());
		entityClickedEvent.SetMouseButton(button);
		eventManager.RaiseEvent(entityClickedEvent);

		entityId = clickedEntity.GetParentEntityID();
	}
}