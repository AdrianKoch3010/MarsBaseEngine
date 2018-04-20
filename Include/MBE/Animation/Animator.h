#pragma once

/// @file
/// @brief Template Class mbe::Animator

#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <map>

#include <MBE/Animation/BaseAnimator.h>
#include <MBE/Core/Utility.h>

namespace mbe
{
	/// @brief Stores an object's animations and their progress
	/// @details Takes care of multiple animations which are registered by an ID.
	/// The animations can be played at any time but only one animation can be played at a time.
	/// @n If the animated object gets destroyed, the animator will be set inactive and have no effect.
	/// If managed by a higher level system such as the mbe::AnimationComponent, it will be deleted once set inactive.
	/// @tparam TAnimated The type of the object being animated. <b>TAnimated must inherit from mbe::HandleBase</b>
	/// @tparam TID The key type used to store and play animations (default is std::string)
	/// @note There is a template overload for TID = std::string
	template <class TAnimated, typename TID = std::string>
	class Animator : public BaseAnimator<TID>
	{
	public:
		/// @brief Functor to animate the objects.
		/// @details TAnimated is the object being animated.
		/// @n The float represents the progress in [0,1] determining the state of the animation
		/// @note This is the definition of an animation. Every animation is seen as a a function altering an object
		/// of type TAnimated over a progress between 0 and 1.
		typedef std::function<void(TAnimated &, float)> AnimationFunction;

	private:
		typedef std::pair<AnimationFunction, sf::Time> ScaledAnimation;
		typedef std::map<TID, ScaledAnimation> AnimationMap;
		typedef typename AnimationMap::iterator	AnimationMapIterator;

	public:
		/// @brief Constructor
		Animator(TAnimated & animatedObject);

		/// @brief Default destructor
		~Animator() = default;

	public:
		/// @brief Updates the animator's progress.
		/// @details This method should be called once each frame. This is done automatically when used in a the mbe::AnimationComponent.
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime) override;

		/// @brief Registers an animation with a given id and duration.
		/// @param id The id that will later be used to play the animation
		/// @param animation The animation to be registered. Any functor of the correct signature suffices
		/// @param duration The absolute duration of the animation in seconds. This parameter can be used to play 'smooth' animations.
		/// For a FrameAnimation The duration should be set to (1 / target frame rate) * number of frames
		void AddAnimation(const TID & id, const AnimationFunction & animation, sf::Time duration);

		/// @brief Plays the animation with the given id.
		/// @param id The id of the animation to be played
		/// @param loop If set to true, the animation will be played repeatedly (until stopped elsewhere). Is set to false by default.
		/// @throws std::runntime_error if no animation has been registed under the given id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		void PlayAnimation(const TID & id, bool loop = false) override;

		void SetPaused(bool value = true) override;

		/// @brief Interrupts the animation that is currently active.
		/// @details The animated object remains in the state of the stopped animation. Calling PlayAnimation() will reset the animation though.
		void StopAnimation() override;

		/// @brief Checks whether an animation is currently playing.
		/// @returns Returns true after an animation has been started with PlayAnimation(), as long as it has not ended. False otherwise.
		bool IsPlayingAnimation() const override;

		bool IsPaused() const override;

		/// @brief Returns the id of the currently playing animation.
		/// @throws std::runntime_error if the no animation is playing.
		/// Therefore, you have to make sure by a preceding call to IsPlayingAnimation()
		/// whether it is safe to call GetPlayingAnimation().
		const TID & GetPlayingAnimation() const override;

		/// @brief Returns true if an animation with this id has been registered
		/// @param id The id of the animation.
		bool HasAnimation(const TID & id) const override;

		/// @brief Sets the animator inactive
		/// @details An inactive is likely to be deleted by a higher level system
		void Destroy() override;

		/// @brief Returns true if this animator is active, false otherwise
		/// @details An animator can be set inactive using the Destroy() method
		bool IsActive() const override;

	private:
		const typename TAnimated::HandleID animatedObjectId;
		bool active;

		AnimationMap animationDictionary;
		AnimationMapIterator currentlyPlayingAnimation;
		float progress;
		bool loop;
		bool paused;
	};

#pragma region Template Overload for TID = std::string

	/// @brief Stores an object's animations and their progress
	/// @details Template overload of the mbe::Animator class when TID = std::string.
	/// @n Takes care of multiple animations which are registered by an id.
	/// The animations can be played at any time but only one animation can be played at a time.
	/// @n If the animated object gets destroyed, the animator will be set inactive and have no effect.
	/// @tparam TAnimated The type of the object being animated. <b>TAnimated must inherit from mbe::HandleBase</b>
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Further they should only use <b>ASCII</b> characters
	template <class TAnimated>
	class Animator<TAnimated, std::string> : public BaseAnimator<std::string>
	{
	public:
		/// @brief Functor to animate the objects.
		/// @details TAnimated is the object being animated.
		/// @n The float represents the progress in [0,1] determining the state of the animation
		/// @note This is the definition of an animation. Every animation is seen as a a function altering an object
		/// of type TAnimated over a progress between 0 and 1.
		typedef std::function<void(TAnimated &, float)> AnimationFunction;

	private:
		typedef std::pair<AnimationFunction, sf::Time> ScaledAnimation;
		typedef std::map<std::string, ScaledAnimation> AnimationMap;
		typedef typename AnimationMap::iterator	AnimationMapIterator;

	public:
		/// @brief Constructor
		Animator(TAnimated & animatedObject);

		/// @brief Default destructor
		~Animator() = default;

	public:
		/// @brief Updates the animator's progress.
		/// @details This method should be called once each frame. This is done automatically when used in a the mbe::AnimationComponent.
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime) override;

		/// @brief Registers an animation with a given id and duration.
		/// @param id The id that will later be used to play the animation.
		/// @param animation The animation to be registered. Any functor of the correct signature suffices
		/// @param duration The absolute duration of the animation in seconds. This parameter can be used to play 'smooth' animations.
		/// For a FrameAnimation The duration should be set to (1 / target frame rate) * number of frames
		void AddAnimation(std::string id, const AnimationFunction & animation, sf::Time duration);

		/// @brief Plays the animation with the given id.
		/// @param id The id of the animation to be played.
		/// @param loop If set to true, the animation will be played repeatedly (until stopped elsewhere). Is set to false by default.
		/// @throws std::runntime_error if no animation has been registed under the given id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		void PlayAnimation(const std::string & id, bool loop = false) override;


		void SetPaused(bool value = true) override;

		/// @brief Interrupts the animation that is currently active.
		/// @details The animated object remains in the state of the stopped animation. Calling PlayAnimation() will reset the animation though.
		void StopAnimation() override;

		/// @brief Checks whether an animation is currently playing.
		/// @returns Returns true after an animation has been started with PlayAnimation(), as long as it has not ended. False otherwise.
		bool IsPlayingAnimation() const override;

		bool IsPaused() const override;

		/// @brief Returns the id of the currently playing animation.
		/// @throws std::runntime_error if the no animation is playing.
		/// Therefore, you have to make sure by a preceding call to IsPlayingAnimation()
		/// whether it is safe to call GetPlayingAnimation().
		const std::string & GetPlayingAnimation() const override;

		/// @brief Returns true if an animation with this id has been registered
		/// @param id The id of the animation.
		bool HasAnimation(const std::string & id) const override;

		/// @brief Sets the animator inactive
		/// @details An inactive is likely to be deleted by a higher level system
		void Destroy() override;

		/// @brief Returns true if this animator is active, false otherwise
		/// @details An animator can be set inactive using the Destroy() method
		bool IsActive() const override;

	private:
		const typename TAnimated::HandleID animatedObjectId;
		bool active;

		AnimationMap animationDictionary;
		AnimationMapIterator currentlyPlayingAnimation;
		float progress;
		bool loop;
		bool paused;
	};

#pragma endregion

#pragma region Template Implementations for Animator<class TAnimated, typename TID>

	template<class TAnimated, typename TID>
	inline Animator<TAnimated, TID>::Animator(TAnimated & animatedObject) :
		animatedObjectId(animatedObject.GetHandleID()),
		active(true),
		animationDictionary(),
		currentlyPlayingAnimation(animationDictionary.end()),
		progress(0.f),
		loop(false),
		paused(false)
	{
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::Update(sf::Time frameTime)
	{
		// If no animation playing, do nothing
		if (!IsPlayingAnimation())
			return;

		// If the animation is paused, do nothing
		if (IsPaused())
			return;

		// Update progress, scale dt with 1 / current animation duration
		progress += frameTime.asSeconds() / currentlyPlayingAnimation->second.second.asSeconds();

		// If animation is expired, stop or restart animation at loops
		if (progress > 1.f)
		{
			if (loop)
				progress -= std::floor(progress); // Store only fractional part
												  // The seperate progress part may not be final
												  /*else
												  StopAnimation();*/
		}
		if (progress > 1.f + frameTime.asSeconds())
			StopAnimation();

		// If an animation is playing, apply it to the Animated template (e.g. the render object of the parentEntity)
		if (this->IsPlayingAnimation())
		{
			auto animatedObjectPtr = TAnimated::GetObjectFromID(animatedObjectId);
			
			// Check if the object still exists
			if (animatedObjectPtr == nullptr)
			{
				// Destroy the animator
				this->Destroy();
				return;
			}

			auto & animationFunction = currentlyPlayingAnimation->second.first;
			animationFunction(*animatedObjectPtr, progress);
		}
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::AddAnimation(const TID & id, const AnimationFunction & animation, sf::Time duration)
	{
		// Make sure that the id is unique
		if (animationDictionary.find(id) != animationDictionary.end())
			throw std::runtime_error("Animator: An animation with the same id already exists");
		animationDictionary.insert(std::make_pair(id, ScaledAnimation(animation, duration)));
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::PlayAnimation(const TID & id, bool loop)
	{
		AnimationMapIterator it = animationDictionary.find(id);

		if (it == animationDictionary.end())
			throw std::runtime_error("Animator: No animation has been registered under this id: " + id);

		currentlyPlayingAnimation = it;
		progress = 0.f;
		this->loop = loop;
		this->SetPaused(false);
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::SetPaused(bool value)
	{
		this->paused = value;
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::StopAnimation()
	{
		currentlyPlayingAnimation = animationDictionary.end();
	}

	template<class TAnimated, typename TID>
	inline bool Animator<TAnimated, TID>::IsPlayingAnimation() const
	{
		return currentlyPlayingAnimation != animationDictionary.end();
	}

	template<class TAnimated, typename TID>
	inline bool Animator<TAnimated, TID>::IsPaused() const
	{
		return paused;
	}

	template<class TAnimated, typename TID>
	inline const TID & Animator<TAnimated, TID>::GetPlayingAnimation() const
	{
		if (this->IsPlayingAnimation() == false)
			throw std::runtime_error("Animator: No animation is currently playing");

		return currentlyPlayingAnimation->first;
	}

	template<class TAnimated, typename TID>
	inline bool Animator<TAnimated, TID>::HasAnimation(const TID & id) const
	{
		return animationDictionary.find(id) != animationDictionary.end();
	}

	template<class TAnimated, typename TID>
	inline void Animator<TAnimated, TID>::Destroy()
	{
		this->active = false;
	}

	template<class TAnimated, typename TID>
	inline bool Animator<TAnimated, TID>::IsActive() const
	{
		return this->active;
	}

#pragma endregion

#pragma region Template Implementations for Animator<class TAnimated, std::string>

	template<class TAnimated>
	inline Animator<TAnimated, std::string>::Animator(TAnimated & animatedObject) :
		animatedObjectId(animatedObject.GetHandleID()),
		active(true),
		animationDictionary(),
		currentlyPlayingAnimation(animationDictionary.end()),
		progress(0.f),
		loop(false),
		paused(false)
	{
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::Update(sf::Time frameTime)
	{
		// If no animation playing, do nothing
		if (!IsPlayingAnimation())
			return;

		// If the animation is paused, do nothing
		if (IsPaused())
			return;

		// Update progress, scale dt with 1 / current animation duration
		progress += frameTime.asSeconds() / currentlyPlayingAnimation->second.second.asSeconds();

		// If animation is expired, stop or restart animation at loops
		if (progress > 1.f)
		{
			if (loop)
				progress -= std::floor(progress); // Store only fractional part
												  // The seperate progress part may not be final
												  /*else
												  StopAnimation();*/
		}
		if (progress > 1.f + frameTime.asSeconds())
			StopAnimation();

		// If an animation is playing, apply it to the Animated template (e.g. the render object of the parentEntity)
		if (this->IsPlayingAnimation())
		{
			auto animatedObjectPtr = TAnimated::GetObjectFromID(animatedObjectId);

			// Check if the object still exists
			if (animatedObjectPtr == nullptr)
			{
				// Destroy the animator
				this->Destroy();
				return;
			}

			auto & animationFunction = currentlyPlayingAnimation->second.first;
			animationFunction(*animatedObjectPtr, progress);
		}
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::AddAnimation(std::string id, const AnimationFunction & animation, sf::Time duration)
	{
		NormaliseIDString(id);

		// Make sure that the id is unique
		if (animationDictionary.find(id) != animationDictionary.end())
			throw std::runtime_error("Animator: An animation with the same id already exists");
		animationDictionary.insert(std::make_pair(id, ScaledAnimation(animation, duration)));
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::PlayAnimation(const std::string & id, bool loop)
	{
		// Convert id to lower case
		auto lowerCaseId = id; // Needed since id is const and function must override mbe::BaseAnimator::PlayAnimation()
		std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);

		AnimationMapIterator it = animationDictionary.find(lowerCaseId);

		if (it == animationDictionary.end())
			throw std::runtime_error("Animator: No animation has been registered under this id: " + id);

		currentlyPlayingAnimation = it;
		progress = 0.f;
		this->loop = loop;
		this->SetPaused(false);
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::SetPaused(bool value)
	{
		this->paused = value;
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::StopAnimation()
	{
		currentlyPlayingAnimation = animationDictionary.end();
	}

	template<class TAnimated>
	inline bool Animator<TAnimated, std::string>::IsPlayingAnimation() const
	{
		return currentlyPlayingAnimation != animationDictionary.end();
	}

	template<class TAnimated>
	inline bool Animator<TAnimated, std::string>::IsPaused() const
	{
		return paused;
	}

	template<class TAnimated>
	inline const std::string & Animator<TAnimated, std::string>::GetPlayingAnimation() const
	{
		if (this->IsPlayingAnimation() == false)
			throw std::runtime_error("Animator: No animation is currently playing");

		return currentlyPlayingAnimation->first;
	}

	template<class TAnimated>
	inline bool Animator<TAnimated, std::string>::HasAnimation(const std::string & id) const
	{
		// Convert id string to lower case
		auto lowerCaseId = id; // Needed since id is const
		std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);

		return animationDictionary.find(lowerCaseId) != animationDictionary.end();
	}

	template<class TAnimated>
	inline void Animator<TAnimated, std::string>::Destroy()
	{
		this->active = false;
	}

	template<class TAnimated>
	inline bool Animator<TAnimated, std::string>::IsActive() const
	{
		return this->active;
	}

#pragma endregion

} // namespace mbe