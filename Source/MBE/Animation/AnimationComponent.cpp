#include <MBE/Animation/AnimationComponent.h>

using namespace mbe;

AnimationComponent::AnimationComponent(EventManager & eventManager, Entity & parentEntity) :
	Component(eventManager, parentEntity)
{
}

void AnimationComponent::Update(sf::Time frameTime)
{
	// Delete inactive animators
	for (auto it = animatorDictionary.cbegin(); it != animatorDictionary.cend();)
	{
		if (it->second->IsActive() == false)
			it = animatorDictionary.erase(it);
		else
			++it;
	}

	// Update all animators
	for (auto & pair : animatorDictionary)
		pair.second->Update(frameTime);
}

typename AnimationComponent::EntityAnimator & AnimationComponent::CreateAnimator(std::string id)
{
	// Convert id string to lower case
	NormaliseIDString(id);

	auto animatorPtr = std::make_unique<EntityAnimator>(this->parentEntity);
	auto & animator = *animatorPtr;
	animatorDictionary.insert(std::make_pair(id, std::move(animatorPtr)));
	return animator;
}

typename AnimationComponent::EntityAnimator & mbe::AnimationComponent::GetAnimator(std::string id)
{
	NormaliseIDString(id);

	const auto it = animatorDictionary.find(id);

	// If the id could not be found throw
	if (it == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + id);

	return *it->second;
}

const typename AnimationComponent::EntityAnimator & AnimationComponent::GetAnimator(std::string id) const
{
	NormaliseIDString(id);

	const auto it = animatorDictionary.find(id);

	// If the id could not be found throw
	if (it == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + id);

	return *it->second;
}

void AnimationComponent::PlayAnimation(std::string animatorId, const std::string & animationId, bool loop)
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);

	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	animatorItr->second->PlayAnimation(animationId, loop);

	//// Recursivly call play Animation for the child entities
	//for (const auto childEntityId : this->GetParentEntity().GetChildEntityIDList())
	//{
	//	assert(Entity::GetObjectFromID(childEntityId) != nullptr && "AnimationComponent: The entity must exist");

	//	/// This will throw if the child entity does not have the animator
	//	auto & childEntity = *Entity::GetObjectFromID(childEntityId);
	//	if (childEntity.HasComponent<mbe::AnimationComponent>())
	//	{
	//		childEntity.GetComponent<mbe::AnimationComponent>().PlayAnimation(animationId, loop);
	//	}
	//}
}

void AnimationComponent::PlayAnimation(const std::string & animationId, bool loop)
{
	bool doesIdExists = false;

	for (auto & pair : animatorDictionary)
	{
		// Only play animations that have been registered (runntime_error thrown otherwise)
		if (pair.second->HasAnimation(animationId))
		{
			doesIdExists = true;
			pair.second->PlayAnimation(animationId, loop);
		}
	}

	// Throw if non of the animators has the animation
	if (doesIdExists == false)
		throw std::runtime_error("AnimationComponent: No animator has an animation with this id: " + animationId);
}

void AnimationComponent::StopAnimation(std::string animatorId, std::string animationId)
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);

	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	auto & animator = animatorItr->second;
	if (animator->IsPlayingAnimation())
	{
		if (animator->GetPlayingAnimation() == animationId)
			animator->StopAnimation();
	}
}

void AnimationComponent::StopAnimation(std::string animationId)
{
	NormaliseIDString(animationId);

	for (auto & pair : animatorDictionary)
	{
		if (pair.second->IsPlayingAnimation())
		{
			if (pair.second->GetPlayingAnimation() == animationId)
				pair.second->StopAnimation();
		}
	}
}

void AnimationComponent::StopAnimation()
{
	for (auto & pair : animatorDictionary)
		pair.second->StopAnimation();
}

void AnimationComponent::SetPaused(std::string animatorId, std::string animationId, bool value)
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	// If the animation is playing, pause it
	auto & animator = *animatorItr->second;
	if (animator.IsPlayingAnimation())
	{
		if (animator.GetPlayingAnimation() == animationId)
			animator.SetPaused(value);
		// else throw?
	}

}

void AnimationComponent::SetPaused(std::string animatorId, bool value)
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	animatorItr->second->SetPaused(value);
}

bool AnimationComponent::IsPlayingAnimation(std::string animatorId, std::string animationId) const
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	// If no animation is playing, false is returned immediately
	return animatorItr->second->IsPlayingAnimation() && animatorItr->second->GetPlayingAnimation() == animationId;
}

bool AnimationComponent::IsPlayingAnimation(std::string animatorId) const
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	return animatorItr->second->IsPlayingAnimation();
}

bool AnimationComponent::IsPlayingAnimation() const
{
	bool playing = false;
	for (const auto & pair : animatorDictionary)
	{
		if (pair.second->IsPlayingAnimation())
			playing = true;
	}
	return playing;
}

bool AnimationComponent::IsPaused(std::string animatorId, std::string animationId) const
{
	NormaliseIDString(animatorId);
	NormaliseIDString(animationId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	auto & animator = *animatorItr->second;
	if (animator.IsPlayingAnimation())
	{
		if (animator.GetPlayingAnimation() == animationId)
			return animator.IsPaused();
	}
	return false;
}

bool AnimationComponent::IsPaused(std::string animatorId) const
{
	NormaliseIDString(animatorId);

	const auto animatorItr = animatorDictionary.find(animatorId);
	if (animatorItr == animatorDictionary.cend())
		throw std::runtime_error("AnimationComponent: No animator exists under this id: " + animatorId);

	return animatorItr->second->IsPaused();
}

std::vector<std::string> AnimationComponent::GetPlayingAnimations() const
{
	std::vector<std::string> playingAnimations;
	for (const auto & pair : animatorDictionary)
	{
		const auto & animator = pair.second;
		if (animator->IsPlayingAnimation())
		{
			playingAnimations.push_back(animator->GetPlayingAnimation());
		}
	}
	return playingAnimations;
}