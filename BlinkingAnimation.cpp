#include "BlinkingAnimation.h"

using namespace mbe;

BlinkingAnimation::BlinkingAnimation(float minimumBrightness)
{
	assert(minimumBrightness >= 0.f && minimumBrightness <= 1.f && "BlinkingAnimation: The minimum brightness must be a value between 0 and 1");
	this->minimumBrightness = minimumBrightness;
}
