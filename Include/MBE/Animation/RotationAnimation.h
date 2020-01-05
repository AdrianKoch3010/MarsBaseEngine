#pragma once

/// @file
/// @brief Class mbe::RotationAnimation

#include <MBE/TransformComponent.h>

namespace mbe
{
	/// @brief A class defining a rotation animation that can be used by the mbe::AnimationComponent
	/// @details Requires the animated object to have a SetRotationFunction
	class RotationAnimation
	{
	public:
		/// @brief Constructor
		/// @param maximumRotation The relative maximum rotation achived. 1 is a full rotation.
		RotationAnimation(float maximumRotation) : relativeMaximumRotation(maximumRotation) {}

		/// @brief Default destructor
		~RotationAnimation() = default;

		/// @brief Applies the animation to the passed in target object
		/// @details The target object must provide a SetRotation(float) method
		/// @tparam Animated The type of the object to apply the animation to
		/// @param target Reference of the target object the animation will be apllied to
		/// @param progress The progress (Values in [0, 1]) determins the state of the animation
		/// i.e. by how much the object is rotated
		template <class TAnimated>
		void operator () (TAnimated & target, float progress) const;

		// @brief Returns the relative maximum rotation
		// @returns The relative maximum rotation achived. 1 is a full rotation.
		inline float GetRelativeMaximumRotation() const { return relativeMaximumRotation; }

	private:
		float relativeMaximumRotation;
	};

#pragma region Template Implementations
	
	template<class TAnimated>
	inline void RotationAnimation::operator()(TAnimated & target, float progress) const
	{
		// The progress part may not be final
		if (progress > 1.f)
			//target.SetRotation(0.f);
			target.GetComponent<TransformComponent>().SetRotation(0.f);
		else
			//target.SetRotation(360.f * relativeMaximumRotation * progress);
			target.GetComponent<TransformComponent>().SetRotation(360.f * relativeMaximumRotation * progress);
	}

#pragma endregion

} // namespace mbe