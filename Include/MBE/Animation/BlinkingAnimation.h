#pragma once

/// @file
/// @brief Class mbe::BlinkingAnimation

#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Graphics/Color.hpp>

namespace mbe
{
	/// Link to the graph used: https://www.graphsketch.com/?eqn1_color=1&eqn1_eqn=0.5%20*%20(sin(2%20*%20pi%20*%20(x%20%2B%200.25))%20*%20(1%20-%200.3)%20%2B%201%20%2B%200.3)&eqn2_color=2&eqn2_eqn=&eqn3_color=3&eqn3_eqn=&eqn4_color=4&eqn4_eqn=&eqn5_color=5&eqn5_eqn=&eqn6_color=6&eqn6_eqn=&x_min=-1&x_max=1&y_min=-1&y_max=1&x_tick=0.1&y_tick=0.1&x_label_freq=5&y_label_freq=5&do_grid=0&do_grid=1&bold_labeled_lines=0&bold_labeled_lines=1&line_width=4&image_w=850&image_h=525
	class BlinkingAnimation
	{
	public:
		// minimum brighness must be a value between 0 and 1
		BlinkingAnimation(float minimumBrightness = 0.f);
		~BlinkingAnimation() = default;

	public:
		template <class TAnimated>
		void operator() (TAnimated & target, float progress);

	private:
		float minimumBrightness;
	};

#pragma region Template Implementations

	template<class TAnimated>
	inline void BlinkingAnimation::operator()(TAnimated & target, float progress)
	{
		assert(progress >= 0.f && progress <= 1.f && "The progress must be a float between 0 and 1");

		// Calculate the brighness
		// 0.5 * (sin(2 * pi * (x + 0.25)) * (1 - y) + 1 + y) where x = progress & y = minimum brightness
		float brightness = 0.5f * (std::sin(2.f * /*M_PI*/ 3.14159265358979323846f * (progress + 0.25f)) * (1.f - minimumBrightness) + 1.f + minimumBrightness);

		// Set the color
		sf::Color color = target.GetColor();
		color.a = static_cast<sf::Uint8>(brightness * 255);
		target.SetColor(color);
	}

#pragma endregion

} // namespace mbe