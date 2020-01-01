#include <MBE/Animation/FrameAnimation.h>

using namespace mbe;

FrameAnimation::FrameAnimation() :
	frameList(),
	normalized(false)
{
}

void FrameAnimation::AddFrame(const sf::IntRect & subrect, float relativeDuration)
{
	frameList.push_back(FrameAnimation::Frame(relativeDuration, subrect));
	normalized = false;
}

void FrameAnimation::AddFrame(const sf::IntRect & subrect, sf::Vector2f origin, float relativeDuration)
{
	frameList.push_back(FrameAnimation::Frame(relativeDuration, subrect, origin));
	normalized = false;
}

void FrameAnimation::AddFrames(sf::Vector2u size, sf::Vector2u first, unsigned short int numberOfFrames)
{
	// for the number of frames to add
	for (size_t i = 0; i < numberOfFrames; i++)
	{
		this->AddFrame(sf::IntRect(size.x * i + first.x, first.y, size.x, size.y));
	}
}

void FrameAnimation::EnsureNormalized() const
{
	if (normalized)
		return;

	float sum = 0.f;
	for (const auto & frame : frameList)
		sum += frame.duration;

	for (const auto & frame : frameList)
		frame.duration /= sum;

	normalized = true;
}
