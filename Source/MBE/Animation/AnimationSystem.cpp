#include <cassert>

#include <MBE/Animation/AnimationSystem.h>

using namespace mbe;

AnimationSystem::AnimationSystem(EntityManager& entityManager) :
	entityManager(entityManager)
{
}

void AnimationSystem::Update(sf::Time frameTime)
{
	for (auto entityId : entityManager.GetComponentGroup<AnimationComponent>())
	{
		assert(Entity::GetObjectFromID(entityId) != nullptr && "AnimationSystem: The entity must exist");
		auto& entity = *Entity::GetObjectFromID(entityId);

		auto& animationComponent = entity.GetComponent<AnimationComponent>();

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

			// If an animation is playing, apply it to the Animated template (e.g. the render object of the parentEntity)
			if (animator.IsPlayingAnimation())
			{
				auto animatedObjectPtr = Entity::GetObjectFromID(entityId);

				auto& currentlyPlayingAnimation = animator.GetAnimationDictionary().at(animator.GetPlayingAnimation());
				auto& animationFunction = currentlyPlayingAnimation.first;
				animationFunction(*animatedObjectPtr, animator.GetProgress());
			}
		}
	}
}