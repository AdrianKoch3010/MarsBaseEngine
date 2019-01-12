#include <MBE/Input/ClickableSystem.h>

using namespace mbe;
using mbe::event::MouseButtonReleasedEvent;

ClickableSystem::ClickableSystem(EventManager & eventManager, EntityManager & entityManager) :
	eventManager(eventManager),
	entityManager(entityManager)
{
	onClickSubscription = eventManager.Subscribe(EventManager::TCallback<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent & event)
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
	const auto & clickableEntities = entityManager.GetComponentGroup<ClickableComponent>();

	for (const auto entityId : clickableEntities)
	{
		/// make sure that the entity exists - this may be done in a refresh function / guaranteed by the entityManager
		assert(Entity::GetObjectFromID(entityId) != nullptr && "ClickableSystem: The entity must exist");
		auto & entity = *Entity::GetObjectFromID(entityId);
		if (entity.HasComponent<ClickableComponent>() == false)
			continue;
		const auto & clickableComponent = entity.GetComponent<ClickableComponent>();

		if (clickableComponent.Contains(CalculatePosition(entity, clickPosition)))
		{
			// Raise the mbe::event::EntityClickedEvent for the clicked entity every entity connected to it
			for (const auto & clickedEntityId : clickableComponent.GetConnectedClickableIdList())
			{
				const auto * clickedEntityPtr = Entity::GetObjectFromID(clickedEntityId);

				// Only raise an entity clicked event for entities that exist
				if (clickedEntityPtr == nullptr)
					continue;

				event::EntityClickedEvent entityClickedEvent;
				entityClickedEvent.SetEntityID(clickedEntityPtr->GetHandleID());
				entityClickedEvent.SetMouseButton(button);
				eventManager.RaiseEvent(entityClickedEvent);
			}
		}
	}
}


sf::Vector2f ClickableSystem::CalculatePosition(const Entity & entity, sf::Vector2f clickPosition)
{
	if (entity.HasComponent<ClickableComponent>() && entity.HasComponent<TransformComponent>() && entity.HasComponent<RenderInformationComponent>())
	{
		const auto & transformComponent = entity.GetComponent<TransformComponent>();
		const auto & renderInformationComponent = entity.GetComponent<RenderInformationComponent>();
		const auto & view = *renderInformationComponent.GetView();

		// Reverse the view transform
		clickPosition = renderInformationComponent.GetRenderWindow()->mapPixelToCoords(static_cast<sf::Vector2i>(clickPosition), view);
		// Reverse the entity transform
		clickPosition = transformComponent.GetWorldTransform().getInverse().transformPoint(clickPosition);
	}
	else if (entity.HasComponent<ClickableComponent>() && entity.HasComponent<TransformComponent>())
	{
		const auto & transformComponent = entity.GetComponent<TransformComponent>();

		// Reverse the entity transform
		clickPosition = transformComponent.GetWorldTransform().getInverse().transformPoint(clickPosition);
	}

	return clickPosition;
}
