#include <mbe/Animation/PitchAnimation.h>

#include <MBE/Serialisation/PitchAnimationSerialiser.h>

using namespace mbe;

void PitchAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration)
{
	using namespace tinyxml2;

	float minimumPitch;

	// Get minimum brightness
	const auto minimumPitchElement = animationData.FirstChildElement("MinimumPitch");
	if (minimumPitchElement == nullptr)
		throw std::runtime_error("Pitch animation serialiser: Failed to parse minimum pitch element");
	if (minimumPitchElement->QueryFloatText(&minimumPitch) != XML_SUCCESS)
		throw std::runtime_error("Pitch animation serialiser: Failed to parse minimum pitch text");

	// Add the animation
	entityAnimator.AddAnimation(animationId, PitchAnimation(minimumPitch), duration);
}

void PitchAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData)
{
	// Get the animation
	const auto& animation = entityAnimator.GetAnimation<PitchAnimation>(animationId);

	auto minimumPitchElement = document.NewElement("MinimumPitch");
	minimumPitchElement->SetText(animation.GetMinimumPitch());
	animationData.InsertEndChild(minimumPitchElement);
}
