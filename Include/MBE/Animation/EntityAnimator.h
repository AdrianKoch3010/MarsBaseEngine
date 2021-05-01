#pragma once

/// @file
/// @brief Class mbe::EntityAnimator

#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <map>
#include <memory>
#include <variant>

#include <MBE/Core/Entity.h>


namespace mbe
{

	namespace detail
	{
		// Note that in this case the MBE_DECLARE_TYPE_ID macro cannot be used since there is no Animation base class
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

	/// @brief Stores an entity's animations and their progress
	/// @details Takes care of multiple animations which are registered by an id.
	/// The animations can be played at any time but only one animation can be played at a time.
	/// @n There is a more general form of Animator that can be used to animate objects that aren't entities.
	/// @see mbe::Animator, mbe::BaseAnimator
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Use the mbe::NormaliseIDString() function when comparing ids. Further they should only use <b>ASCII</b> characters.
	class EntityAnimator
	{
	public:
		/// @brief Signature of the type of function used to animate the entity.
		/// @details The float represents the progress in [0,1] determining the state of the animation
		/// @note This is the definition of an animation. Every animation is seen as a a function altering an entity
		/// over a progress between 0 and 1.
		typedef std::function<void(Entity&, float)> AnimationFunction;

		/// @brief The type of the animation type id
		/// @details Each type of animation function has a unqiue number specifying its type
		typedef detail::AnimationTypeID AnimationTypeID;

		typedef std::shared_ptr<EntityAnimator> Ptr;
		typedef std::weak_ptr<EntityAnimator> WPtr;
		typedef std::unique_ptr<EntityAnimator> UPtr;

	private:
		typedef std::pair<std::variant<AnimationFunction, std::string>, sf::Time> ScaledAnimation;
		typedef std::map<std::string, ScaledAnimation> AnimationDictionary;
		typedef typename AnimationDictionary::iterator	AnimationDictionaryIterator;
		typedef std::unordered_map<std::string, detail::AnimationTypeID> AnimationTypeDictionary;
		typedef std::unordered_map<std::string, std::string> StringDictionary;

	public:
		/// @brief Constructor
		/// @param entity A reference to the entity that is being animated. 
		/// @details Typically, the entity animator will live inside the animation component which itself is part of the entity.
		/// Therefore, this reference is guaranteed to be valid since when the entity gets destroyed, so do its component and, hence, the entity animator.
		/// By default the progress is set to 0 and both the looped and paused properties to false.
		EntityAnimator(Entity& entity);

		/// @brief Default destructor
		~EntityAnimator() = default;

	public:
		/// @brief Registers an animation with a given id and duration.
		/// @param id The id that will later be used to play the animation.
		/// @param animation The animation to be registered. Any functor of the correct signature suffices
		/// @param duration The absolute duration of the animation in seconds. This parameter can be used to play 'smooth' animations.
		/// For a FrameAnimation The duration should be set to (1 / target frame rate) * number of frames
		/// @tparam TAnimation The type of the animation to be registerd. It must be a function object of type AnimationFunction and
		/// provide a static GetTypeID() method.
		template <typename TAnimation>
		void AddLocalAnimation(std::string id, const TAnimation& animation, sf::Time duration);

		/// @brief Registers an animation with a given id and duration.
		/// @param id The id that will later be used to play the animation
		/// @param globalId The id that refers to the animation template
		/// @param duration The absolute duration of the animation in seconds. This parameter can be used to play 'smooth' animations.
		/// For a FrameAnimation The duration should be set to (1 / target frame rate) * number of frames
		void AddGlobalAnimation(std::string id, std::string globalId, sf::Time duration);

		/// @brief Plays the animation with the given id.
		/// @param id The id of the animation to be played.
		/// @param loop If set to true, the animation will be played repeatedly (until stopped elsewhere). Is set to false by default.
		/// @throws std::runntime_error if no animation has been registered under this id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		void PlayAnimation(const std::string& id, bool loop = false);

		/// @brief Pauses or unpauses the currently playing animation
		/// @param value Pass true to pause, false to unpause
		void SetPaused(bool value = true);

		/// @brief Sets the currently playing animation looping
		/// @param value Pass True to set looping, false to stop looping
		inline void SetLooping(bool value = true) { loop = value; }

		/// @brief Set the progress of the currenty playing animation
		/// @note The progress is automatically increased when the animation is playing.
		/// @param value The progress as a float value between 0 and 1
		/// @throws std::runtime_error if the value is outside this range
		void SetProgress(float value);

		/// @brief Interrupts the animation that is currently active.
		/// @details The animated object remains in the state of the stopped animation. Calling PlayAnimation() will reset the animation though.
		void StopAnimation();

		/// @brief Checks whether an animation is currently playing.
		/// @returns Returns true after an animation has been started with PlayAnimation(), as long as it has not ended. False otherwise.
		bool IsPlayingAnimation() const;

		/// @brief Returns whether the currently playing animation is paused
		/// @returns Ture if yes, false otherwise
		bool IsPaused() const;

		/// @brief Returns whether the current animation is looping
		/// @returns True if the animation is looping, false otherwise
		inline bool IsLooping() const { return loop; }

		/// @brief Returns the progress of the currently playing animation
		/// @returns The progress as value between 0 and 1
		inline float GetProgress() const { return progress; }

		/// @brief Returns the id of the currently playing animation.
		/// @throws std::runntime_error if the no animation is playing.
		/// Therefore, you have to make sure by a preceding call to IsPlayingAnimation()
		/// whether it is safe to call GetPlayingAnimation().
		const std::string& GetPlayingAnimation() const;

		/// @brief Returns true if an animation with this id has been registered
		/// @param id The id of the animation.
		bool HasAnimation(const std::string& id) const;

		/// @brief Gets animation of a specified type and id
		/// @tparam TAnimation The type of the requested animation. Animations are function objects.
		/// @param id The id of the requested animation.
		/// @returns The animation casted to TAnimation
		/// @note Only animations that are stroed locally in this animator can be accessed. Animations that are stored using a global
		/// animation id must be looked up in their respective mbe::AnimationHolder
		/// @throws std::runntime_error if no animation has been registered under this id or if the requested animation is not of type TAnimation.
		/// Therefore, you have to make sure by a preceding call to HasAnimation() that an animation with this id and type exists.
		template<typename TAnimation>
		const TAnimation& GetLocalAnimation(const std::string& id) const;

		/// @brief Gets the type id of an animation
		/// @details The type id of an animation is a unique number for that animation type. This can be useful when comparing the
		/// types of two animations.
		/// @param id The id of the animation
		/// @retruns The type of the animtion
		/// @note Only animations that are stroed locally in this animator can be accessed. Animations that are stored using a global
		/// animation id must be looked up in their respective mbe::AnimationHolder
		/// @throws std::runntime_error if no animation has been registered under this id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		AnimationTypeID GetLocalAnimationTypeID(const std::string& id) const;

		/// @Gets the duration of an animation
		/// @param id The id of the requested animation
		/// @returns the duration as a sf::Time object
		/// @throws std::runntime_error if no animation has been registered under this id. Therefore, you have to make sure
		/// by a preceding call to HasAnimation() that an animation with this id exists.
		sf::Time GetAnimationDuration(const std::string& id) const;

		/// @brief Gets the underlying animation dictionary
		/// @details This method is meant for the mbe::AnimationSystem and mbe::AnimationComponentSerialiser to have direct access to the underlying dictionary
		/// and should generally be avoided. To change the state of the playing animation, use id = GetPlayingAnimation() followed by e.g. PlayAnimation(id) instead.
		/// @returns A reference to the underlying animation dictionary
		inline AnimationDictionary& GetAnimationDictionary() { return animationDictionary; }

		/// @see GetAnimationDictionary
		inline const AnimationDictionary& GetAnimationDictionary() const { return animationDictionary; }

		/// @brief Returns whether a local to global animation id mapping exists
		/// @details The global animation id refers to an animation template and is unique across an mbe::AnimationHolder.
		/// The animation id is only unqiue within this animator and can be seen as a more general name of the animation.
		/// Note that it can be useful to use the same name across multiple animators to play multiple animations through a single play call.
		/// @param animationId The (local) id or name of the animation
		/// @return True if the animation has been added via a global animation id, false otherwise
		bool HasGlobalAnimationID(std::string animationId) const;

		/// @brief Returns the corresponding global animation id
		/// @throws If the animation has not been added as a global animation id
		/// @see HasGlobalAnimationID()
		/// @param animationID The (local) id or name of the animation
		/// @return The global id of the animation (if it exists)
		const std::string& GetGlobalAnimationID(std::string animationID) const;

	private:
		const Entity::ID entityId;

		AnimationDictionary animationDictionary;
		AnimationDictionaryIterator currentlyPlayingAnimation;
		AnimationTypeDictionary animationTypeDictionary;

		float progress;
		bool loop;
		bool paused;

		StringDictionary globalAnimationIdDictionary;
	};

#pragma region Template Implementations

	template<typename TAnimation>
	inline void EntityAnimator::AddLocalAnimation(std::string id, const TAnimation& animation, sf::Time duration)
	{
		NormaliseIDString(id);

		// Make sure that the id is unique
		if (animationDictionary.find(id) != animationDictionary.cend())
			throw std::runtime_error("EntityAnimator: An animation with the same id already exists (" + id + ")");

		animationDictionary.insert(std::make_pair(id, ScaledAnimation(animation, duration)));

		// Remember the type
		//animationTypeDictionary.insert(std::make_pair(id, detail::GetAnimationTypeID<TAnimationFunction>()));
		animationTypeDictionary.insert(std::make_pair(id, TAnimation::GetTypeID<TAnimation>()));
	}

	template<typename TAnimation>
	inline const TAnimation& EntityAnimator::GetLocalAnimation(const std::string& id) const
	{
		if (HasAnimation(id) == false)
			throw std::runtime_error("Entity animator: No animation has been added under this id (" + id + ")");

		// The animation must be stored as a local animation
		if (animationDictionary.at(id).first.index() != 0)
			throw std::runtime_error("Entity animator: This animation is stored in an animation holder");

		// Try to cast the animaton to the required type
		auto animationPtr = std::get<AnimationFunction>(animationDictionary.at(id).first).target<TAnimation>();

		if (animationPtr == nullptr)
			throw std::runtime_error("Entity animator: Wrong animation type");

		return *animationPtr;
	}

#pragma endregion

} // namespace mbe