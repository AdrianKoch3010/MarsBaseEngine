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

	// Ptr since entity exist
	std::vector<const Entity *> clickedEntityList;

	for (const auto entityId : clickableEntities)
	{
		/// make sure that the entity exists - this may be done in a refresh function / guaranteed by the entityManager
		assert(Entity::GetObjectFromID(entityId) != nullptr && "ClickableSystem: The entity must exist");
		const auto & entity = *Entity::GetObjectFromID(entityId);

		const auto & clickableComponent = entity.GetComponent<ClickableComponent>();

		if (clickableComponent.Contains(CalculatePosition(entity, clickPosition)))
		{
			// If the entity has a renderInformationComponent the 'drawing' order + clickAbsorbtion must be taken into account
			if (entity.HasComponent<RenderInformationComponent>())
				clickedEntityList.push_back(&entity);
			else
				RaiseClickEvents(clickableComponent, button);
		}
	}

	// Sort the clicked entity list by render order (in decending order)
	std::sort(clickedEntityList.begin(), clickedEntityList.end(), [](const Entity * a, const Entity * b) {
		return b->GetComponent<RenderInformationComponent>().IsAboveOrEqual(a->GetComponent<RenderInformationComponent>());
	});


	// Loop through all clicked entities and figure out for which the event must be raised (depending on which one is drawn on top of each other)
	for (const auto * entityPtr : clickedEntityList)
	{
		const auto & clickableComponent = entityPtr->GetComponent<ClickableComponent>();
		
		RaiseClickEvents(clickableComponent, button);

		if (clickableComponent.IsClickAbsorebd())
			return;
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

void ClickableSystem::RaiseClickEvents(const ClickableComponent & clickableComponent, sf::Mouse::Button button)
{
	//for (const auto & clickedEntityId : clickableComponent.GetConnectedClickableIdList())
	//{
	//	const auto * clickedEntityPtr = Entity::GetObjectFromID(clickedEntityId);

	//	// Only raise an entity clicked event for entities that exist
	//	if (clickedEntityPtr == nullptr)
	//		continue;

	//	event::EntityClickedEvent entityClickedEvent;
	//	entityClickedEvent.SetEntityID(clickedEntityPtr->GetHandleID());
	//	entityClickedEvent.SetMouseButton(button);
	//	eventManager.RaiseEvent(entityClickedEvent);
	//}

	if (clickableComponent.DoesBubbleUp() == false)
	{
		event::EntityClickedEvent entityClickedEvent;
		entityClickedEvent.SetEntityID(clickableComponent.GetParentEntity().GetHandleID());
		entityClickedEvent.SetMouseButton(button);
		eventManager.RaiseEvent(entityClickedEvent);
		return;
	}

	for (auto entityId = clickableComponent.GetParentEntity().GetHandleID(); Entity::GetObjectFromID(entityId) != nullptr; )
	{
		const auto & clickedEntity = *Entity::GetObjectFromID(entityId);
		
		event::EntityClickedEvent entityClickedEvent;
		entityClickedEvent.SetEntityID(clickedEntity.GetHandleID());
		entityClickedEvent.SetMouseButton(button);
		eventManager.RaiseEvent(entityClickedEvent);

		entityId = clickedEntity.GetParentEntityID();
	}
}

//// A diffent function signature would be required to recursively call the function for parent entities that don't necessarily have a Clickable Component
//void ClickableSystem::RaiseConnectedClickEvents(const ClickableComponent & clickableComponent, sf::Mouse::Button button)
//{
//	const auto parentEntityId = clickableComponent.GetParentEntity().GetParentEntityID();
//	if (parentEntityId != Entity::GetNullID())
//	{
//		// The entity should exist since if the parent is deleted the child entities are deleted as well
//		assert(Entity::GetObjectFromID(parentEntityId) != nullptr && "Clickable System: The parent entity must exist");
//
//		event::EntityClickedEvent entityClickedEvent;
//		entityClickedEvent.SetEntityID(parentEntityId);
//		entityClickedEvent.SetMouseButton(button);
//		eventManager.RaiseEvent(entityClickedEvent);
//
//		// Recursive call
//		RaiseConnectedClickEvents(....);
//	}
//}
