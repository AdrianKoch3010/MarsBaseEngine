#include <MBE/Animation/EntityAnimator.h>

using namespace mbe;

EntityAnimator::EntityAnimator(Entity& entity) :
	entityId(entity.GetHandleID()),
	animationDictionary(),
	currentlyPlayingAnimation(animationDictionary.end()),
	progress(0.f),
	loop(false),
	paused(false)
{
}

void EntityAnimator::PlayAnimation(const std::string& id, bool loop)
{
	// Convert id to lower case
	auto lowerCaseId = id; // Needed since id is const and function must override mbe::BaseAnimator::PlayAnimation()
	std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);

	AnimationDictionaryIterator it = animationDictionary.find(lowerCaseId);

	if (it == animationDictionary.end())
		throw std::runtime_error("EntityAnimator: No animation has been registered under this id: " + id);

	currentlyPlayingAnimation = it;
	progress = 0.f;
	this->loop = loop;
	this->SetPaused(false);
}

void EntityAnimator::SetPaused(bool value)
{
	this->paused = value;
}

void EntityAnimator::SetProgress(float value)
{
	if (progress < 0.f || progress > 1.f)
		throw std::runtime_error("EntityAnimator: The progress must be a value between 0 and 1");

	progress = value;
}

void EntityAnimator::StopAnimation()
{
	currentlyPlayingAnimation = animationDictionary.end();
}


bool EntityAnimator::IsPlayingAnimation() const
{
	return currentlyPlayingAnimation != animationDictionary.end();
}

bool EntityAnimator::IsPaused() const
{
	return paused;
}

const std::string& EntityAnimator::GetPlayingAnimation() const
{
	if (this->IsPlayingAnimation() == false)
		throw std::runtime_error("EntityAnimator: No animation is currently playing");

	return currentlyPlayingAnimation->first;
}

bool EntityAnimator::HasAnimation(const std::string& id) const
{
	// Convert id string to lower case
	auto lowerCaseId = id; // Needed since id is const
	std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);

	return animationDictionary.find(lowerCaseId) != animationDictionary.end();
}

EntityAnimator::AnimationTypeID EntityAnimator::GetAnimationTypeID(const std::string& animationId) const
{
	if (animationTypeDictionary.find(animationId) == animationTypeDictionary.end())
		throw std::runtime_error("EntityAnimator: No animation has been registered under this id: " + animationId);

	return animationTypeDictionary.at(animationId);
}
