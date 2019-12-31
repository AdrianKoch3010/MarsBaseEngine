#pragma once

/// @file
/// @brief Template Class mbe::Animator

#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <map>
#include <memory>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Utility.h>

//namespace mbe
//{
//	class Animation
//	{
//	public:
//		typedef std::shared_ptr<Animation> Ptr;
//		typedef std::weak_ptr<Animation> WPtr;
//		typedef std::unique_ptr<Animation> UPtr;
//
//	public:
//		Animation() = default;
//		virtual ~Animation() = default;
//
//	public:
//		virtual void Animate(Entity& entity, float progress) = 0;
//	};
//
//} // namespace mbe


namespace mbe
{

	namespace detail
	{
		typedef std::size_t AnimationTypeID;

		constexpr AnimationTypeID UnspecifiedAnimtionTypeID = std::numeric_limits<AnimationTypeID>::max();

		inline AnimationTypeID GetAnimationID() noexcept
		{
			// This will only be initialised once
			static AnimationTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		template <typename T>
		inline AnimationTypeID GetAnimationTypeID() noexcept
		{
			// There will be only one static variable for each template type
			static AnimationTypeID typeId = GetAnimationID();
			return typeId;
		}
	}

	/// @brief Stores an object's animations and their progress
	/// @details Template overload of the mbe::Animator class when TID = std::string.
	/// @n Takes care of multiple animations which are registered by an id.
	/// The animations can be played at any time but only one animation can be played at a time.
	/// @n If the animated object gets destroyed, the animator will be set inactive and have no effect.
	/// @tparam TAnimated The type of the object being animated. <b>TAnimated must inherit from mbe::HandleBase</b>
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Further they should only use <b>ASCII</b> characters
	class EntityAnimator
	{
	public:
		/// @brief Functor to animate the objects.
		/// @details TAnimated is the object being animated.
		/// @n The float represents the progress in [0,1] determining the state of the animation
		/// @note This is the definition of an animation. Every animation is seen as a a function altering an object
		/// of type TAnimated over a progress between 0 and 1.
		typedef std::function<void(Entity&, float)> AnimationFunction;

		typedef std::shared_ptr<EntityAnimator> Ptr;
		typedef std::weak_ptr<EntityAnimator> WPtr;
		typedef std::unique_ptr<EntityAnimator> UPtr;

		typedef detail::AnimationTypeID AnimationTypeID;

	private:
		typedef std::pair<AnimationFunction, sf::Time> ScaledAnimation;
		typedef std::map<std::string, ScaledAnimation> AnimationDictionary;
		typedef typename AnimationDictionary::iterator	AnimationDictionaryIterator;
		typedef std::unordered_map<std::string, detail::AnimationTypeID> AnimationTypeDictionary;

	public:
		/// @brief Constructor
		EntityAnimator(Entity& entity);

		/// @brief Default destructor
		~EntityAnimator() = default;

	public:
		/// @brief Registers an animation with a given id and duration.
		/// @param id The id that will later be used to play the animation.
		/// @param animation The animation to be registered. Any functor of the correct signature suffices
		/// @param duration The absolute duration of the animation in seconds. This parameter can be used to play 'smooth' animations.
		/// For a FrameAnimation The duration should be set to (1 / target frame rate) * number of frames
		template <typename TAnimationFunction>
		void AddAnimation(std::string id, const TAnimationFunction& animation, sf::Time duration);

		/// @brief Plays the animation with the given id.
		/// @param id The id of the animation to be played.
		/// @param loop If set to true, the animation will be played repeatedly (until stopped elsewhere). Is set to false by default.
		/// @throws std::runntime_error if no animation has been registed under the given id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		void PlayAnimation(const std::string& id, bool loop = false);


		void SetPaused(bool value = true);

		// Set the currently playing animation looping
		inline void SetLooping(bool value = true) { loop = value; }

		// Set the progress of the currenty playing animation
		// The progress is automatically increased when the animation is playing
		// The progress is a float value between 0 and 1
		// Throws if outside the range
		void SetProgress(float value);

		/// @brief Interrupts the animation that is currently active.
		/// @details The animated object remains in the state of the stopped animation. Calling PlayAnimation() will reset the animation though.
		void StopAnimation();

		/// @brief Checks whether an animation is currently playing.
		/// @returns Returns true after an animation has been started with PlayAnimation(), as long as it has not ended. False otherwise.
		bool IsPlayingAnimation() const;

		bool IsPaused() const;

		// Returns true if the current animation is looping
		inline bool IsLooping() const { return loop; }

		// Returns the progress (between 0 and 1) of the currently playing animation
		inline float GetProgress() const { return progress; }

		/// @brief Returns the id of the currently playing animation.
		/// @throws std::runntime_error if the no animation is playing.
		/// Therefore, you have to make sure by a preceding call to IsPlayingAnimation()
		/// whether it is safe to call GetPlayingAnimation().
		const std::string& GetPlayingAnimation() const;

		/// @brief Returns true if an animation with this id has been registered
		/// @param id The id of the animation.
		bool HasAnimation(const std::string& id) const;

		inline AnimationDictionary& GetAnimationDictionary() { return animationDictionary; }

		inline const AnimationDictionary& GetAnimationDictionary() const { return animationDictionary; }

		// throws std::runntime_error if no animation has been registed under the given id. Therefore, you have to make sure
		// by a preceding call to HasAnimation() that an animation with this id exists.
		AnimationTypeID GetAnimationTypeID(const std::string& animationId) const;

	private:
		const Entity::HandleID entityId;

		AnimationDictionary animationDictionary;
		AnimationDictionaryIterator currentlyPlayingAnimation;
		AnimationTypeDictionary animationTypeDictionary;

		float progress;
		bool loop;
		bool paused;
	};

#pragma region Template Implementations

	template<typename TAnimationFunction>
	inline void EntityAnimator::AddAnimation(std::string id, const TAnimationFunction& animation, sf::Time duration)
	{
		NormaliseIDString(id);

		// Make sure that the id is unique
		if (animationDictionary.find(id) != animationDictionary.end())
			throw std::runtime_error("EntityAnimator: An animation with the same id already exists");
		
		// Remember the type
		animationTypeDictionary.insert(std::make_pair(id, detail::GetAnimationTypeID<TAnimationFunction>()));

		animationDictionary.insert(std::make_pair(id, ScaledAnimation(animation, duration)));
	}

#pragma endregion

} // namespace mbe