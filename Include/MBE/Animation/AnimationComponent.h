#pragma once

/// @file
/// @brief Class mbe::AnimationComponent

#include <string>
#include <set>
#include <unordered_map>

#include <MBE/Animation/EntityAnimator.h>
#include <MBE/Core/Component.h>
#include <MBE/Core/Entity.h>
#include <MBE/Core/Utility.h>

namespace mbe
{

	/// @brief Stores an entities animations
	/// @details Provides methods for creating and accessing animators as well as changing the animation's state directly through the play, stop and pause animation methods.
	/// Animations must be added to an animator, not the animation component. Each animator can have one active (playing or paused) animation meaning that more than one animation
	/// can be played simultaniuosly, provided they have been added to different animators.
	/// @note The PlayAnimation(), StopAnimation() and SetPaused() methods do not throw an exception if the id of the animator / animation hasn't been found for this component's.
	/// Instead, The method is called recursivly for all the child entities (of this component's parent entity) that also have an animation component.
	/// Therefore, an animator can be manipulated on the child entity even if it has never been registered for this entity. If none of the child entities has the requested 
	/// animator / animation no action is taken. If a suitable child entity is added later on, the actions will have an effect from that point onwards.
	/// This property will be refered to as the recursive property
	/// throughout this class' docuementation.
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Further they should only use <b>ASCII</b> characters
	/// @see mbe::EntityAnimator
	class AnimationComponent : public Component
	{
	private:
		typedef std::unordered_map<std::string, EntityAnimator::UPtr> AnimatorDictionary;

	public:
		/// @brief Constructor
		/// @param eventManager A reference to the mbe::EventManager
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		AnimationComponent(EventManager& eventManager, Entity& parentEntity);

		/// @brief Default destructor
		~AnimationComponent() = default; // Delete the BaseAnimators

	public:
		/// @brief Creates an animator and adds it to the list of animators managed by this component
		/// @param id The id of the animator that that can be used to refer to it later
		/// @returns A reference to the entity animator created.
		EntityAnimator& CreateAnimator(std::string id);

		// Only returns a refernce to this entities animator so the recursive functions are not supported
		/// @brief Gets an animator by id
		/// @details This method does not posses the recursive property. It exists purely for convenience and should not be needed.
		/// To play, pause, stop or check whether an animation of a specific animator is playing or paused use the respective overloads of the
		/// PlayAnimation(), StopAnimation(), SetPaused() and IsPlayingAnimation() and IsPaused() methods.
		/// @param id The id under which the animator has been registered
		/// @returns A reference to the requested animator.
		/// @throws std::runtime_error if no animator could be found with this id
		EntityAnimator& GetAnimator(std::string id);

		// Const overload
		/// @copydoc GetAnimator
		const EntityAnimator& GetAnimator(std::string id) const;

		/// @brief Returns true if an animator with this id has been registered
		/// @param id The id under which the animatior has been registered
		/// @returns True if an animator under this id has been registed to this entity or any of its child entities, false otherwise
		/// @see CreateAnimator, GetAnimator, HasAnimation
		bool HasAnimator(std::string id) const;

		/// @brief Returns true if any of the registerd animators has an animation with this id
		/// @details This method iterates over all registered animators. It posses the recursive property.
		/// @param id The id under which the animation has been added
		/// @returns True if an animation with this id has been added to any registred animators (either of this entity or any of its child entities), false otherwise
		/// @see CreateAnimator, GetAnimator, HasAnimator
		bool HasAnimation(std::string id) const;

		// See: https://stackoverflow.com/questions/18463937/how-to-prevent-implicit-conversion-from-char-array-to-bool
		// Carefull to not call PlayAnimation(string, string) --> this will call PlayAnimation(string, bool)!!!!!
		/// @brief Attemts to play an animation on an animator
		/// @details The PlayAnimation() method is called recursivly for all the child entities that have an animation component.
		/// Therefore, an animation can be played on the child entity even though it has never been registered for this entity.
		/// @note If no animator has been registered under the animator id or if no animation has been added to that animation
		/// under the animation id, no action is taken.
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @param loop When set to true, the animtion keeps playing in a loop until it is paused or stopped
		/// @see StopAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void PlayAnimation(std::string animatorId, const std::string& animationId, bool loop); // Loop can't be default initialised to false --> overload gets confused

		/// @brief Attempts to play an animation on all animators
		/// @details Gets every animator that has the requested animation and plays it.
		/// This can be used to either play an animation when the animator id is not known or when playing multiple animations 
		/// with the same id (but different animators). The PlayAnimation() method is called recursivly for all the child entities that have an animation component.
		/// Therefore, an animation can be played on the child entity even though it has never been registered for this entity.
		/// @note If no animation has been found under the animation id, no action is taken.
		/// @param animationId The id used when adding the animation to the animator
		/// @param loop When set to true, the animtion keeps playing in a loop until it is paused or stopped
		/// @see StopAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void PlayAnimation(const std::string& animationId, bool loop = false);

		/// @brief Attemps to stop a specific animation on an animator
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @note There is a difference between a paused and a stopped animation. A paused animation can be stopped, but a stopped animation cannot be paused.
		/// An animation must be playing in order for it to be paused.
		/// @see PlayAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void StopAnimation(std::string animatorId, std::string animationId);

		/// @brief Attemps to stop the currently playing animation on an animator
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @note There is a difference between a paused and a stopped animation. A paused animation can be stopped, but a stopped animation cannot be paused.
		/// An animation must be playing in order for it to be paused.
		/// @see PlayAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void StopAnimation(std::string animatorId);

		/// @brief Attempts to stop the currently playing animation on all animators
		/// @details This method posses the recursive property
		/// @note There is a difference between a paused and a stopped animation. A paused animation can be stopped, but a stopped animation cannot be paused.
		/// An animation must be playing in order for it to be paused.
		/// @see PlayAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void StopAnimation();

		/// @brief Attempts to pause or unpause a specific animation of an animator
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @param value True to pause, false to unpause
		/// @note An animation must be playing in order to be paused
		/// @see PlayAnimation, StopAnimation, IsPlayingAnimation, IsPaused
		void SetPaused(std::string animatorId, std::string animationId, bool value); // Loop can't be default initialised to false --> overload gets confused

		/// @brief Attempts to pause or unpause the currently playing animation on an animator
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @param value True to pause, false to unpause
		/// @note An animation must be playing in order to be paused
		/// @see PlayAnimation, StopAnimation, IsPlayingAnimation, IsPaused
		void SetPaused(std::string animatorId, bool value = true);

		/// @brief Returns whether an animator is playing a specific animation
		/// @details This method posses the recursive property
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @returns True if the animation is currently playing on any registred animator (either of this entity or any of its child entities), false otherwise
		/// @note A playing animation could be paused.
		/// @see PlayAnimation, StopAnimation, SetPaused, IsPaused
		bool IsPlayingAnimation(std::string animatorId, std::string animationId) const;

		/// @brief Returns whether an animator is playing an animation
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @returns True if an animation is currently playing on any registred animator (either of this entity or any of its child entities), false otherwise
		/// @note A playing animation could be paused.
		/// @see PlayAnimation, StopAnimation, SetPaused, IsPaused
		bool IsPlayingAnimation(std::string animatorId) const;

		/// @brief Returns whether any animator is playing an animation
		/// @details This method posses the recursive property.
		/// @returns True if any animator (either of this entity or any of its child entities) is currently playing an animation, false otherwise
		/// @note A playing animation could be paused.
		/// @see PlayAnimation, StopAnimation, SetPaused, IsPaused
		bool IsPlayingAnimation() const;

		/// @brief Returns whether a specific animation of an animator is paused
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @param animationId The id used when adding the animation to the animator
		/// @retuns True if the animator is currently playing the animation and it is paused, false otherwise
		/// @note An animation must be playing in order to be paused
		/// @see PlayAnimation, StopAnimation, IsPlayingAnimation, SetPaused
		bool IsPaused(std::string animatorId, std::string animationId) const;

		/// @brief Returns whether animator's currently playing animation is paused
		/// @details This method posses the recursive property.
		/// @param animatorId The id under which the animator has been registered
		/// @returns True if the animator (either of this entity or any of its child entities) is currently playing an animation that is paused, false otherwise
		/// @note An animation must be playing in order to be paused
		/// @see PlayAnimation, StopAnimation, IsPlayingAnimation, SetPaused
		bool IsPaused(std::string animatorId) const;

		/// @brief Gets a list of the ids of the currently playing animations on this entity
		/// @details This methods does not posses the recursive property.
		/// @returns A list of the ids of the currently playing animations on this entity
		/// @note A playing animation could be paused.
		std::vector<std::string> GetPlayingAnimations() const;

		/// @brief Get the internal animator dictionary
		/// @details This is only needed when efficient access to all of the entity's animators is required.
		/// To play, pause, stop or check whether an animation of a specific animator is playing or paused use the respective overloads of the
		/// PlayAnimation(), StopAnimation(), SetPaused() and IsPlayingAnimation() and IsPaused() methods.
		/// @returns A reference to the animator dictionary for this entity
		inline AnimatorDictionary& GetAnimatorDictionary() { return animatorDictionary; }

		/// @copydoc GetAnimatorDictionary
		inline const AnimatorDictionary& GetAnimatorDictionary() const { return animatorDictionary; }

	private:
		AnimatorDictionary animatorDictionary;
	};

} // namespace mbe