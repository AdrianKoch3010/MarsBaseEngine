#include <MBE/Animation/PitchAnimation.h>

using namespace mbe;

PitchAnimation::PitchAnimation(float minimumPitch)
{
	assert(minimumPitch >= 0.f && minimumPitch <= 1.f && "PitchAnimation: The minimum pitch must be a value between 0 and 1");
	this->minimumPitch = minimumPitch;
}
