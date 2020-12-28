#pragma once

/// @file
/// @brief Class mbe::PitchAnimation

#include <memory>
#include <cassert>
#include <cmath>

#include <MBE/Core/Utility.h>
#include <MBE/Animation/EntityAnimator.h>
#include <MBE/Audio/BaseAudioComponent.h>

namespace mbe
{

	class PitchAnimation
	{
	public:
		typedef std::shared_ptr<PitchAnimation> Ptr;
		typedef std::weak_ptr<PitchAnimation> WPtr;
		typedef std::unique_ptr<PitchAnimation> UPtr;

		typedef typename EntityAnimator::AnimationTypeID TypeID;

	public:
		// minimum brighness must be a value between 0 and 1
		PitchAnimation(float minimumPitch = 0.f);
		~PitchAnimation() = default;

	public:
		template <class TAnimated>
		void operator() (TAnimated & target, float progress);

		inline float GetMinimumPitch() const { return minimumPitch; }

		MBE_GET_TYPE_ID(Animation)

	private:
		float minimumPitch;
	};

#pragma region Template Implementations

	// Link to the graph used: https://www.graphsketch.com/?eqn1_color=1&eqn1_eqn=0.5%20*%20(sin(2%20*%20pi%20*%20(x%20%2B%200.25))%20*%20(1%20-%200.3)%20%2B%201%20%2B%200.3)&eqn2_color=2&eqn2_eqn=&eqn3_color=3&eqn3_eqn=&eqn4_color=4&eqn4_eqn=&eqn5_color=5&eqn5_eqn=&eqn6_color=6&eqn6_eqn=&x_min=-1&x_max=1&y_min=-1&y_max=1&x_tick=0.1&y_tick=0.1&x_label_freq=5&y_label_freq=5&do_grid=0&do_grid=1&bold_labeled_lines=0&bold_labeled_lines=1&line_width=4&image_w=850&image_h=525
	template<class TAnimated>
	inline void PitchAnimation::operator()(TAnimated & target, float progress)
	{
		assert(progress >= 0.f && progress <= 1.f && "The progress must be a float between 0 and 1");

		// Calculate the brighness
		// 0.5 * (sin(2 * pi * (x + 0.25)) * (1 - y) + 1 + y) where x = progress & y = minimum brightness
		float pitch = 0.5f * (std::sin(2.f * 3.14159265358979323846f * (progress + 0.25f)) * (1.f - minimumPitch) + 1.f + minimumPitch);

		//target.GetAudioData().pitch = pitch;
		target.GetComponent<BaseAudioComponent>().SetPitch(pitch);
	}

#pragma endregion

} // namespace mbe