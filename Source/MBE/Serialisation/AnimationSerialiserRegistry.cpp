#include <MBE/Serialisation/AnimationSerialiserRegistry.h>

// Animations and serialisers
#include <MBE/Animation/FrameAnimation.h>
#include <MBE/Serialisation/FrameAnimationSerialiser.h>
#include <MBE/Animation/RotationAnimation.h>
#include <MBE/Serialisation/RotationAnimationSerialiser.h>
#include <MBE/Animation/BlinkingAnimation.h>
#include <MBE/Serialisation/BlinkingAnimationSerialiser.h>
#include <MBE/Animation/PitchAnimation.h>
#include <MBE/Serialisation/PitchAnimationSerialiser.h>

using namespace mbe;

AnimationSerialiserRegistry::AnimationSerialiserRegistry()
{
	AddAnimationSerialiser<FrameAnimationSerialiser, FrameAnimation>(MBE_NAME_OF(FrameAnimation));
	AddAnimationSerialiser<RotationAnimationSerialiser, RotationAnimation>(MBE_NAME_OF(RotationAnimation));
	AddAnimationSerialiser<BlinkingAnimationSerialiser, BlinkingAnimation>(MBE_NAME_OF(BlinkingAnimation));
	AddAnimationSerialiser<PitchAnimationSerialiser, PitchAnimation>(MBE_NAME_OF(PitchAnimation));
}