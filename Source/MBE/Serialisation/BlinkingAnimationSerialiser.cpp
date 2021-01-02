#include <mbe/Animation/BlinkingAnimation.h>

#include <MBE/Serialisation/BlinkingAnimationSerialiser.h>

using namespace mbe;

void BlinkingAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{	// Add the animation
	// Casting the result of Parse is necessary in order to register the correct function type
	entityAnimator.AddLocalAnimation(animationId, *Parse(animationData).target<BlinkingAnimation>(), duration);
}

void BlinkingAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const
{
	// Get the animation
	const auto& animation = entityAnimator.GetLocalAnimation<BlinkingAnimation>(animationId);

	auto minimumBrightnessElement = document.NewElement("MinimumBrightness");
	minimumBrightnessElement->SetText(animation.GetMinimumBrightness());
	animationData.InsertEndChild(minimumBrightnessElement);
}

EntityAnimator::AnimationFunction BlinkingAnimationSerialiser::Parse(const tinyxml2::XMLElement& animationData) const
{
	using namespace tinyxml2;

	float minimumBrightness;

	// Get minimum brightness
	const auto minimumBrightnessElement = animationData.FirstChildElement("MinimumBrightness");
	if (minimumBrightnessElement == nullptr)
		throw ParseError(MBE_NAME_OF(BlinkingAnimationSerialiser), "MinimumBrightness node is required", animationData.GetLineNum());
	if (minimumBrightnessElement->QueryFloatText(&minimumBrightness) != XML_SUCCESS)
		throw ParseError(MBE_NAME_OF(BlinkingAnimationSerialiser), "Failed to parse MinimumBrightness text", minimumBrightnessElement->GetLineNum());

	return BlinkingAnimation(minimumBrightness);
}