#include <mbe/Animation/RotationAnimation.h>

#include <MBE/Serialisation/RotationAnimationSerialiser.h>

using namespace mbe;

//MBE_REGISTER_ANIMATION_SERIALISER(RotationAnimationSerialiser, RotationAnimation)

//namespace AnimationSerialiserRegistrations
//{
//	mbe::AnimationSerialiserRegistrations::AnimationSerialiserRegistration<RotationAnimationSerialiser, RotationAnimation> frameAniamtion(MBE_NAME_OF(RotationAnimation));
//}


void RotationAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{
	// Add the animation
	entityAnimator.AddLocalAnimation(animationId, *Parse(animationData).target<RotationAnimation>(), duration);
}

void RotationAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const
{
	// Get the animation
	const auto& animation = entityAnimator.GetLocalAnimation<RotationAnimation>(animationId);

	auto maximumRotationElement = document.NewElement("RelativeMaximumRotation");
	maximumRotationElement->SetText(animation.GetRelativeMaximumRotation());
	animationData.InsertEndChild(maximumRotationElement);
}

EntityAnimator::AnimationFunction RotationAnimationSerialiser::Parse(const tinyxml2::XMLElement& animationData) const
{
	using namespace tinyxml2;

	float maximumRotation;

	// Get minimum brightness
	const auto maximumRotationElement = animationData.FirstChildElement("RelativeMaximumRotation");
	if (maximumRotationElement == nullptr)
		throw ParseError(MBE_NAME_OF(RotationAnimationSerialiser), "RelativeMaximumRotation node is required", animationData.GetLineNum());
	if (maximumRotationElement->QueryFloatText(&maximumRotation) != XML_SUCCESS)
		throw ParseError(MBE_NAME_OF(RotationAnimationSerialiser), "Failed to parse RelativeMaximumRotation text", maximumRotationElement->GetLineNum());

	return RotationAnimation(maximumRotation);
}
