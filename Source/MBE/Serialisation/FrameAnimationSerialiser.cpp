#include <MBE/Serialisation/FrameAnimationSerialiser.h>

#include <MBE/Animation/FrameAnimation.h>
#include <MBE/Core/Utility.h>

using namespace mbe;


void FrameAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{
	// Add the animation
	entityAnimator.AddLocalAnimation(animationId, Parse(animationData), duration);
}

void FrameAnimationSerialiser::Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const
{
	// Get the frame list
	for (const auto& frame : entityAnimator.GetLocalAnimation<FrameAnimation>(animationId).GetFrameList())
	{
		auto frameElement = document.NewElement("Frame");
		animationData.InsertEndChild(frameElement);

		// Store duration
		auto durationElement = document.NewElement("Duration");
		durationElement->SetText(frame.duration);
		frameElement->InsertEndChild(durationElement);

		// Store origin (if needed)
		if (frame.applyOrigin)
		{
			auto originElement = document.NewElement("Origin");
			originElement->SetAttribute("x", frame.origin.x);
			originElement->SetAttribute("y", frame.origin.y);
			frameElement->InsertEndChild(originElement);
		}

		// Store subRect
		auto subRectElement = document.NewElement("SubRect");
		IntRectSerialiser::Store(frame.subrect, document, *subRectElement);
		frameElement->InsertEndChild(subRectElement);
	}
}

EntityAnimator::AnimationFunction FrameAnimationSerialiser::Parse(const tinyxml2::XMLElement& animationData) const
{
	using namespace tinyxml2;

	// Create the animation
	FrameAnimation frameAnimation;

	// Load the list of frames
	for (auto frameElement = animationData.FirstChildElement("Frame"); frameElement != nullptr; frameElement = frameElement->NextSiblingElement("Frame"))
	{
		float duration;
		bool setOrigin;
		sf::Vector2f origin;

		// Get Duration
		const auto durationElement = frameElement->FirstChildElement("Duration");
		if (durationElement == nullptr)
			throw std::runtime_error("Load frame animation: Failed to parse duration element");
		if (durationElement->QueryFloatText(&duration) != XML_SUCCESS)
			throw std::runtime_error("Load frame animation: Failed to parse duration text");

		// Get Origin
		// The origin is optional
		const auto originElement = frameElement->FirstChildElement("Origin");
		if (originElement == nullptr)
			setOrigin = false;
		else
		{
			setOrigin = true;
			if (originElement->QueryFloatAttribute("x", &origin.x) != XML_SUCCESS)
				throw std::runtime_error("Load frame animation: Failed to pass origin x attribute");
			if (originElement->QueryFloatAttribute("y", &origin.y) != XML_SUCCESS)
				throw std::runtime_error("Load frame animation: Failed to pass origin y attribute");
		}

		// Get the subrect
		const auto subRectElement = frameElement->FirstChildElement("SubRect");
		if (subRectElement == nullptr)
			throw std::runtime_error("Load frame animation: Failed to parse SubRect element");
		auto subRect = IntRectSerialiser::Load(*subRectElement);

		// Add the frame to the frame animation
		if (setOrigin)
			frameAnimation.AddFrame(subRect, origin, duration);
		else
			frameAnimation.AddFrame(subRect, duration);
	}

	return frameAnimation;
}
