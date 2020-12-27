#include <mbe/Animation/PitchAnimation.h>

#include <MBE/Serialisation/PitchAnimationSerialiser.h>

using namespace mbe;

//MBE_REGISTER_ANIMATION_SERIALISER(PitchAnimationSerialiser, PitchAnimation)

//namespace AnimationSerialiserRegistrations
//{
//	mbe::AnimationSerialiserRegistrations::AnimationSerialiserRegistration<PitchAnimationSerialiser, PitchAnimation> frameAniamtion(MBE_NAME_OF(PitchAnimation));
//}

void PitchAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{
	// Add the animation
	entityAnimator.AddLocalAnimation(animationId, Parse(animationData), duration);
}

void PitchAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const
{
	// Get the animation
	const auto& animation = entityAnimator.GetLocalAnimation<PitchAnimation>(animationId);

	auto minimumPitchElement = document.NewElement("MinimumPitch");
	minimumPitchElement->SetText(animation.GetMinimumPitch());
	animationData.InsertEndChild(minimumPitchElement);
}

EntityAnimator::AnimationFunction PitchAnimationSerialiser::Parse(const tinyxml2::XMLElement& animationData) const
{
	using namespace tinyxml2;

	float minimumPitch;

	// Get minimum brightness
	const auto minimumPitchElement = animationData.FirstChildElement("MinimumPitch");
	if (minimumPitchElement == nullptr)
		throw std::runtime_error("Pitch animation serialiser: Failed to parse minimum pitch element");
	if (minimumPitchElement->QueryFloatText(&minimumPitch) != XML_SUCCESS)
		throw std::runtime_error("Pitch animation serialiser: Failed to parse minimum pitch text");

	return PitchAnimation(minimumPitch);
}
