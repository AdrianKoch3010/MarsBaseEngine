#include <mbe/Animation/BlinkingAnimation.h>

#include <MBE/Serialisation/BlinkingAnimationSerialiser.h>

using namespace mbe;

void BlinkingAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration)
{
	using namespace tinyxml2;

	float minimumBrightness;

	// Get minimum brightness
	const auto minimumBrightnessElement = animationData.FirstChildElement("MinimumBrightness");
	if (minimumBrightnessElement == nullptr)
		throw std::runtime_error("Blinking animation serialiser: Failed to parse minimum brightness element");
	if (minimumBrightnessElement->QueryFloatText(&minimumBrightness) != XML_SUCCESS)
		throw std::runtime_error("Blinking animation serialiser: Failed to parse minimum brightness text");

	// Add the animation
	entityAnimator.AddAnimation(animationId, BlinkingAnimation(minimumBrightness), duration);
}

void BlinkingAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData)
{
	// Get the animation
	const auto& animation = entityAnimator.GetAnimation<BlinkingAnimation>(animationId);

	auto minimumBrightnessElement = document.NewElement("MinimumBrightness");
	minimumBrightnessElement->SetText(animation.GetMinimumBrightness());
	animationData.InsertEndChild(minimumBrightnessElement);
}
