#include <mbe/Animation/RotationAnimation.h>

#include <MBE/Serialisation/RotationAnimationSerialiser.h>

using namespace mbe;

void RotationAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{
	// Add the animation
	entityAnimator.AddLocalAnimation(animationId, Parse(animationData), duration);
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
		throw std::runtime_error("Rotation animation serialiser: Failed to parse relative maximum rotation element");
	if (maximumRotationElement->QueryFloatText(&maximumRotation) != XML_SUCCESS)
		throw std::runtime_error("Rotation animation serialiser: Failed to parse relative maximum rotation text");

	return RotationAnimation(maximumRotation);
}
