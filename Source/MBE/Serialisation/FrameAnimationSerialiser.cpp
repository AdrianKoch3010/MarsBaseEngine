#include <MBE/Serialisation/FrameAnimationSerialiser.h>

#include <MBE/Animation/FrameAnimation.h>
#include <MBE/Core/Utility.h>

using namespace mbe;


void FrameAnimationSerialiser::Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const
{
	// Add the animation
	entityAnimator.AddLocalAnimation(animationId, *Parse(animationData).target<FrameAnimation>(), duration);
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
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Duration node is required", animationData.GetLineNum());
		if (durationElement->QueryFloatText(&duration) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse Duration text", durationElement->GetLineNum());

		// Get Origin
		// The origin is optional
		const auto originElement = frameElement->FirstChildElement("Origin");
		if (originElement == nullptr)
			setOrigin = false;
		else
		{
			setOrigin = true;
			if (originElement->QueryFloatAttribute("x", &origin.x) != XML_SUCCESS)
				throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse Origin x attribute", originElement->GetLineNum());
			if (originElement->QueryFloatAttribute("y", &origin.y) != XML_SUCCESS)
				throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse Origin y attribute", originElement->GetLineNum());
		}

		// Get the subrect
		const auto subRectElement = frameElement->FirstChildElement("SubRect");
		if (subRectElement == nullptr)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "SubRect node is required", animationData.GetLineNum());
		auto subRect = IntRectSerialiser::Load(*subRectElement);

		// Add the frame to the frame animation
		if (setOrigin)
			frameAnimation.AddFrame(subRect, origin, duration);
		else
			frameAnimation.AddFrame(subRect, duration);
	}

	for (auto framesElement = animationData.FirstChildElement("Frames"); framesElement != nullptr; framesElement = framesElement->NextSiblingElement("Frames"))
	{
		sf::Vector2u size, first;
		unsigned int noOfFrames = 0;

		// Get the size
		const auto sizeElement = framesElement->FirstChildElement("Size");
		if (sizeElement == nullptr)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Size node is required", framesElement->GetLineNum());
		if (sizeElement->QueryUnsignedAttribute("x", &size.x) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse Size x attribute", sizeElement->GetLineNum());
		if (sizeElement->QueryUnsignedAttribute("y", &size.y) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse Size y attribute", sizeElement->GetLineNum());

		// Get the first node
		const auto firstElement = framesElement->FirstChildElement("First");
		if (firstElement == nullptr)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "First node is required", framesElement->GetLineNum());
		if (firstElement->QueryUnsignedAttribute("x", &first.x) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse First x attribute", firstElement->GetLineNum());
		if (firstElement->QueryUnsignedAttribute("y", &first.y) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse First y attribute", firstElement->GetLineNum());

		// Get the noOfFrames
		const auto noOfFramesElement = framesElement->FirstChildElement("NoOfFrames");
		if (noOfFramesElement == nullptr)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "NoOfFrames node is required", framesElement->GetLineNum());
		if (noOfFramesElement->QueryUnsignedText(&noOfFrames) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(FrameAnimationSerialiser), "Failed to parse NoOfFrames text", noOfFramesElement->GetLineNum());

		frameAnimation.AddFrames(size, first, noOfFrames);
	}

	return frameAnimation;
}
