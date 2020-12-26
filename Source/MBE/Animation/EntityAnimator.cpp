#include <MBE/Core/Utility.h>

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

void EntityAnimator::AddGlobalAnimation(std::string id, std::string globalId, sf::Time duration)
{
	NormaliseIDString(id);
	NormaliseIDString(globalId);

	// Make sure that the id is unique
	if (animationDictionary.find(id) != animationDictionary.end())
		throw std::runtime_error("EntityAnimator: An animation with the same id already exists (" + id + ")");

	animationDictionary.insert(std::make_pair(id, ScaledAnimation(globalId, duration)));

	// Register the id in the global id dictionary
	globalAnimationIdDictionary.insert({ id, globalId });
}

void EntityAnimator::PlayAnimation(const std::string& id, bool loop)
{
	auto normalisedId = NormaliseIDString(id);

	AnimationDictionaryIterator it = animationDictionary.find(normalisedId);

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
	auto normalisedId = NormaliseIDString(id);

	return animationDictionary.find(normalisedId) != animationDictionary.end();
}

sf::Time EntityAnimator::GetAnimationDuration(const std::string& id) const
{
	if (HasAnimation(id) == false)
		throw std::runtime_error("Entity animator: No animation has been added under this id (" + id + ")");

	return animationDictionary.at(id).second;
}

EntityAnimator::AnimationTypeID EntityAnimator::GetLocalAnimationTypeID(const std::string& id) const
{
	auto normalisedId = NormaliseIDString(id);

	if (animationTypeDictionary.find(normalisedId) == animationTypeDictionary.end())
		throw std::runtime_error("EntityAnimator: No animation has been registered under this id: " + id);

	return animationTypeDictionary.at(normalisedId);
}

bool EntityAnimator::HasGlobalAnimationID(std::string animationId) const
{
	NormaliseIDString(animationId);
	auto it = globalAnimationIdDictionary.find(animationId);
	if (it != globalAnimationIdDictionary.cend())
		return true;

	return false;
}

const std::string& EntityAnimator::GetGlobalAnimationID(std::string animationID) const
{
	NormaliseIDString(animationID);
	if (HasGlobalAnimationID(animationID) == false)
		throw std::runtime_error("Animator: No global animation id exists for this id: " + animationID);

	return globalAnimationIdDictionary.at(animationID);
}
