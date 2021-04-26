#include <cassert>
#include <variant>

#include <MBE/Animation/AnimationSystem.h>

using namespace mbe;

AnimationSystem::AnimationSystem(EntityManager& entityManager, AnimationHolder& animationHolder) :
	entityManager(entityManager),
	animationHolder(animationHolder)
{
}

void AnimationSystem::Update(sf::Time frameTime)
{
	for (auto& entityId : entityManager.GetComponentGroup<AnimationComponent>())
	{
		auto& animationComponent = entityId->GetComponent<AnimationComponent>();

		// For each animator
		for (auto& pair : animationComponent.GetAnimatorDictionary())
		{
			assert(pair.second != nullptr && "AnimationSystem: The animator must exist");
			auto& animator = *pair.second;

			// If no animation playing, do nothing
			if (animator.IsPlayingAnimation() == false)
				continue;

			// If the animation is paused, do nothing
			if (animator.IsPaused())
				continue;

			// Update progress, scale dt with 1 / current animation duration
			auto progress = animator.GetProgress();
			auto& currentlyPlayingAnimation = animator.GetAnimationDictionary().at(animator.GetPlayingAnimation());
			progress += frameTime.asSeconds() / currentlyPlayingAnimation.second.asSeconds();

			// If animation is expired, stop or restart animation at loops
			if (progress > 1.f)
			{
				if (animator.IsLooping())
				{
					// Store only fractional part
					progress -= std::floor(progress);
				}
				else
				{
					animator.StopAnimation();
					continue;
				}
			}

			animator.SetProgress(progress);

			// If an animation is playing, apply it to the entity
			if (animator.IsPlayingAnimation())
			{
				auto& currentlyPlayingAnimation = animator.GetAnimationDictionary().at(animator.GetPlayingAnimation());

				// If this animation references the animation holder
				if (auto globalId = std::get_if<std::string>(&currentlyPlayingAnimation.first))
				{
					const auto& animationFunction = animationHolder[*globalId];
					animationFunction(*entityId, animator.GetProgress());
				}
				else
				{
					const auto& animationFunction = std::get<EntityAnimator::AnimationFunction>(currentlyPlayingAnimation.first);
					animationFunction(*entityId, animator.GetProgress());
				}
			}
		}
	}
}