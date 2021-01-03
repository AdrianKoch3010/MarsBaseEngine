#pragma once

/// @file
/// @brief Class mbe::FrameAnimation

#include <SFML\Graphics\Rect.hpp>

#include <vector>
#include <cassert>
#include <memory>

#include <MBE/Core/Utility.h>
#include <MBE/Animation/EntityAnimator.h>
#include <MBE/TransformComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>

namespace mbe
{
	/// @brief Changes a texture wrapper component's subrect over time.
	/// @details This class stores multiple frames that represent the sub-rectangle of a texture.
	/// The resulting animation consists of a sequence of frames that are drawn one after another.
	class FrameAnimation
	{
	public:
		/// @brief Class that stores a single frame of FrameAnimation
		struct Frame
		{
			Frame(float duration, const sf::IntRect& subrect) :
				duration(duration),
				subrect(subrect),
				origin(),
				applyOrigin(false)
			{
			}

			Frame(float duration, const sf::IntRect& subrect, sf::Vector2f origin) :
				duration(duration),
				subrect(subrect),
				origin(origin),
				applyOrigin(true)
			{
			}

			mutable float	duration;
			sf::IntRect		subrect;
			sf::Vector2f	origin;
			const bool		applyOrigin;
		};

	public:
		typedef std::shared_ptr<FrameAnimation> Ptr;
		typedef std::weak_ptr<FrameAnimation> WPtr;
		typedef std::unique_ptr<FrameAnimation> UPtr;

		typedef typename EntityAnimator::AnimationTypeID TypeID;

	public:
		/// @brief Default constructor
		FrameAnimation();

		/// @brief Default Destructor
		~FrameAnimation() = default;

		/// @brief Adds a frame to the animation, changes only the sub-rect of the sf::Texture.
		/// @param subrect The sub-rect of the sf::Texture
		/// @param relativeDuration Duration relative to the other frame durations (1.0 by default)
		void AddFrame(const sf::IntRect& subrect, float relativeDuration = 1.f);

		/// @brief Adds a frame to the animation, changes sub-rect of the sf::Texture and the sprite origin.
		/// @param subrect The sub-rect of the sf::Texture
		/// @param origin Position of the coordinate system origin. Is useful when frames have sub-rects of different sizes.
		/// @param relativeDuration The duration relative to the other frame durations (1.0 by default).
		void AddFrame(const sf::IntRect& subrect, sf::Vector2f origin, float relativeDuration = 1.f);

		/// @brief Add several frames of equal size and default origin
		/// @details Assumes that all frames are equal in size and are located next to each other (left to right) on the sprite sheet texture.
		/// The relative duration will be set to 1 meaning that all frames will have the same duration.
		/// @param size The size of each sub-rect
		/// @param first The position (top-left) of the first sub-rect
		/// @param numberOfFrames The number of frames to be added
		/// @warning Make sure that the sprite sheet texture (to which the animation is later applied) is big enough!
		void AddFrames(sf::Vector2u size, sf::Vector2u first, unsigned short int numberOfFrames);

		/// @brief Returns the list of frames
		inline const std::vector<Frame>& GetFrameList() const { return frameList; }

		/// @brief Applies the animation to the passed in target object
		/// @details The target object must provide a SetTextureRect(sf::IntRect) and SetOrigin(sf::Vector2f) method
		/// @tparam Animated The type of the object to apply the animation to
		/// @param target Reference of the target object the animation will be apllied to
		/// @param progress The progress (Values in [0, 1]) determins the state of the animation i.e. which sub-rect is applied
		template <class TAnimated>
		void operator () (TAnimated & target, float progress) const;

		MBE_GET_TYPE_ID(Animation)

	private:
		void EnsureNormalized() const;

	private:
		std::vector<Frame> frameList;
		mutable bool normalized;
	};

#pragma region Template Implementations

	template<class TAnimated>
	inline void FrameAnimation::operator()(TAnimated & target, float progress) const
	{
		assert(!frameList.empty() && "FrameAnimation: No frames exist that could be applied");
		assert(progress >= 0.f && progress <= 1.f && "FrameAnimation: The progress must be a float between 0 and 1");

		EnsureNormalized();
		for (const auto & frame : frameList)
		{
			progress -= frame.duration;

			if (progress < 0.f)
			{
				//target.SetTextureRect(frame.subrect);
				target.GetComponent<TextureWrapperComponent>().SetTextureRect(frame.subrect);
				if (frame.applyOrigin)
					//target.SetOrigin(frame.origin);
					target.GetComponent<TransformComponent>().SetOrigin(frame.origin);
				return;
			}
		}
	}

#pragma endregion

} // namespace mbe