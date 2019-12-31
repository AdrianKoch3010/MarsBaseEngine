#include <MBE/Animation/AnimationComponent.h>

using namespace mbe;

AnimationComponent::AnimationComponent(EventManager& eventManager, Entity& parentEntity) :
	Component(eventManager, parentEntity)
{
}

EntityAnimator& AnimationComponent::CreateAnimator(std::string id)
{
	// Convert id string to lower case
	NormaliseIDString(id);

	auto animatorPtr = std::make_unique<EntityAnimator>(this->parentEntity);
	auto& animator = *animatorPtr;
	animatorDictionary.insert(std::make_pair(id, std::move(animatorPtr)));
	return animator;
}

EntityAnimator& AnimationComponent::GetAnimator(std::string id)
{
	NormaliseIDString(id);

	const auto it = animatorDictionary.find(id);

	// If the id could not be found throw
	if (it == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + id);

	return *it->second;
}

const EntityAnimator& AnimationComponent::GetAnimator(std::string id) const
{
	NormaliseIDString(id);

	const auto it = animatorDictionary.find(id);

	// If the id could not be found throw
	if (it == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + id);

	return *it->second;
}

bool AnimationComponent::HasAnimator(std::string id) const
{
	NormaliseIDString(id);
	const auto it = animatorDictionary.find(id);

	if (it != animatorDictionary.cend())
		return true;

	// Recursivly search if any of the child entities has this animator
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().HasAnimator(id))
				return true;
		}
	}

	return false;
}

bool AnimationComponent::HasAnimation(std::string id) const
{
	for (auto& pair : animatorDictionary)
	{
		if (pair.second->HasAnimation(id))
			return true;
	}

	// Recursivly search if any of the child entities has this animator
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().HasAnimation(id))
				return true;
		}
	}

	return false;
}

void AnimationComponent::PlayAnimation(std::string animatorId, const std::string& animationId, bool loop)
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);

	// Only play the animation if the animator and animation exist
	if (animatorItr != animatorDictionary.cend() && animatorItr->second->HasAnimation(animationId))
		animatorItr->second->PlayAnimation(animationId, loop);

	// Recursivly call play Animation for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<mbe::AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().PlayAnimation(animatorId, animationId, loop);
	}
}

void AnimationComponent::PlayAnimation(const std::string& animationId, bool loop)
{
	for (auto& pair : animatorDictionary)
	{
		// Only play animations that have been registered
		if (pair.second->HasAnimation(animationId))
			pair.second->PlayAnimation(animationId, loop);
	}

	// Recursivly call play Animation for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().PlayAnimation(animationId, loop);
	}
}

void AnimationComponent::StopAnimation(std::string animatorId, std::string animationId)
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);

	// Only stop the animation if the animator exists and its playing the animation to stop
	if (animatorItr != animatorDictionary.cend())
	{
		auto& animator = animatorItr->second;
		if (animator->IsPlayingAnimation())
		{
			if (animator->GetPlayingAnimation() == animationId)
				animator->StopAnimation();
		}
	}

	// Recursivly call stop animation for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().StopAnimation(animatorId, animationId);
	}
}

void AnimationComponent::StopAnimation(std::string animatorId)
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);

	// Only stop the animation if the animator exists and its playing the animation to stop
	if (animatorItr != animatorDictionary.cend())
	{
		auto& animator = animatorItr->second;
		if (animator->IsPlayingAnimation())
		{
			animator->StopAnimation();
		}
	}

	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().StopAnimation(animatorId);
	}
}

void AnimationComponent::StopAnimation()
{
	for (auto& pair : animatorDictionary)
		pair.second->StopAnimation();

	// Recursivly call stop animation for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().StopAnimation();
	}
}

void AnimationComponent::SetPaused(std::string animatorId, std::string animationId, bool value)
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	// Pause / unpause the animation if found
	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr != animatorDictionary.cend())
	{
		// If the animation is playing, pause it
		auto& animator = *animatorItr->second;
		if (animator.IsPlayingAnimation())
		{
			if (animator.GetPlayingAnimation() == animationId)
				animator.SetPaused(value);
		}
	}

	// Recursivly call set paused for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().SetPaused(animatorId, animationId, value);
	}
}

void AnimationComponent::SetPaused(std::string animatorId, bool value)
{
	NormaliseIDString(animatorId);

	// Pause / unpause the animator if found
	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr != animatorDictionary.cend())
		animatorItr->second->SetPaused(value);

	// Recursivly call set paused for the child entities
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
			childEntity.GetComponent<AnimationComponent>().SetPaused(animatorId, value);
	}
}

bool AnimationComponent::IsPlayingAnimation(std::string animatorId, std::string animationId) const
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr != animatorDictionary.cend())
	{
		// If no animation is playing, false is returned immediately
		if (animatorItr->second->IsPlayingAnimation() && animatorItr->second->GetPlayingAnimation() == animationId)
			return true;
	}

	// Recursivly search if any of the child entities is playing this animation
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().IsPlayingAnimation(animatorId, animationId))
				return true;
		}
	}
	return false;
}

bool AnimationComponent::IsPlayingAnimation(std::string animatorId) const
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
	{
		if (animatorItr->second->IsPlayingAnimation())
			return true;
	}

	// Recursivly search if any of the child entities is playing this animation
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().IsPlayingAnimation(animatorId))
				return true;
		}
	}

	return false;
}

bool AnimationComponent::IsPlayingAnimation() const
{
	for (const auto& pair : animatorDictionary)
	{
		if (pair.second->IsPlayingAnimation())
			return true;
	}

	// Recursivly search if any of the child entities is playing this animation
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().IsPlayingAnimation())
				return true;
		}
	}

	return false;
}

bool AnimationComponent::IsPaused(std::string animatorId, std::string animationId) const
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr != animatorDictionary.cend())
	{
		auto& animator = *animatorItr->second;
		if (animator.IsPlayingAnimation())
		{
			if (animator.GetPlayingAnimation() == animationId && animator.IsPaused())
				return true;
		}
	}

	// Recursivly search if any of the child entities is paused
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().IsPaused(animatorId, animationId))
				return true;
		}
	}

	return false;
}

bool AnimationComponent::IsPaused(std::string animatorId) const
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
	{
		if (animatorItr->second->IsPaused())
			return true;
	}

	// Recursivly search if any of the child entities is paused
	for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	{
		assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

		auto& childEntity = *Entity::GetObjectFromID(childEntityId);
		if (childEntity.HasComponent<AnimationComponent>())
		{
			if (childEntity.GetComponent<AnimationComponent>().IsPaused(animatorId))
				return true;
		}
	}

	return false;
}

std::vector<std::string> AnimationComponent::GetPlayingAnimations() const
{
	std::vector<std::string> playingAnimations;
	for (const auto& pair : animatorDictionary)
	{
		const auto& animator = pair.second;
		if (animator->IsPlayingAnimation())
		{
			playingAnimations.push_back(animator->GetPlayingAnimation());
		}
	}
	return playingAnimations;
}