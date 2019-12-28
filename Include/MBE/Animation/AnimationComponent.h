#pragma once

/// @file
/// @brief Class mbe::AnimationComponent

#include <string>
#include <set>
#include <unordered_map>

#include <MBE/Animation/Animator.h>
#include <MBE/Core/Component.h>
#include <MBE/Core/Entity.h>
#include <MBE/Core/Utility.h>

namespace mbe
{

	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Further they should only use <b>ASCII</b> characters
	class AnimationComponent : public Component
	{
	private:
		typedef Animator<Entity> EntityAnimator;

		typedef std::unordered_map<std::string, EntityAnimator::UPtr> AnimatorDictionary;

	public:
		/// @brief Constructor
		/// @param eventManager A reference to the mbe::EventManager
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		AnimationComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief Default destructor
		~AnimationComponent() = default; // Delete the BaseAnimators

	public:

		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
		/// @param frameTime The delta time between two successive frames
		void Update(sf::Time frameTime) override;

		// Add the templated id later on
		/// @brief Creates an animator and adds it to the list of animators managed by this component
		/// @tparam TAnimated The type of object being animated. This will determine the type of the animator that is created
		/// and the type of the animation functions that can be added to it.
		/// @param id The id of the animator that will later be used to acess it
		/// @param animatedObject A reference to the object that the created animator will animate. It must inherit from mbe::HandleBase.
		/// If the animated object gets destroyed, the animator will be deleted. It is generally good practice to animate only objects that are part of this entity
		/// @returns A reference of the animator created.
		/// @attention When the animated object gets deleted the animator animating it is deleted as well. Hence, references of the animator may become invalid.
		/// For this reason animations should only be added when creating the animator. If for some reason a reference must be kept, use Animator<TAnimated>::GetHandleID().
		EntityAnimator & CreateAnimator(std::string id);

		// Only returns a refernce to this entities animator so the recursive functions are not supported
		EntityAnimator & GetAnimator(std::string id);

		// Const overload
		/// @brief Searches for an animator with the given id
		/// @details This function exists purely for convenience and should not be needed. To play, pause, stop or check whether an animation of a specific animator
		/// is playing or paused use the respective overloads of the PlayAnimation(), StopAnimation(), SetPaused() and IsPlayingAnimation() and IsPaused() methods.
		/// @param The id of the animator to get
		/// @returns A reference of the requested animator.
		/// @throws if no animator could be found
		const EntityAnimator & GetAnimator(std::string id) const;

		/// @brief Returns true if an animator with this id has been registered
		/// @param id The id under which the animatior has been registered
		/// @returns True if an animator under this id has been registed, false otherwise
		/// @see CreateAnimator, GetAnimator
		bool HasAnimator(std::string id) const;

		/// @brief Returns true if any of the registerd animators has an animation with this id
		/// @details This method iterates over all registered animators.
		/// @param id The id under which the animation has been added
		/// @returns True if an animation with this id has been added to any registred animator
		bool HasAnimation(std::string id) const;

		//template <class TAnimated>
		//const Animator<TAnimated>::HandleID GetAnimator(std::string id) const;

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
		void PlayAnimation(std::string animatorId, const std::string & animationId, bool loop); // Loop can't be default initialised to false --> overload gets confused

		/// @brief Attempts to play an animation on all animators
		/// @details Gets every animator that has the requested animation and plays it.
		/// This can be used to either play an animation when the animator id is not known or when playing multiple animations 
		/// with the same id (but different animators). The PlayAnimation() method is called recursivly for all the child entities that have an animation component.
		/// Therefore, an animation can be played on the child entity even though it has never been registered for this entity.
		/// @note If no animation has been found under the animation id, no action is taken.
		/// @param animationId The id used when adding the animation to the animator
		/// @param loop When set to true, the animtion keeps playing in a loop until it is paused or stopped
		/// @see StopAnimation, SetPaused, IsPlayingAnimation, IsPaused
		void PlayAnimation(const std::string & animationId, bool loop = false);

		void StopAnimation(std::string animatorId, std::string animationId);

		void StopAnimation(std::string animatorId);

		void StopAnimation();

		void SetPaused(std::string animatorId, std::string animationId, bool value); // Loop can't be default initialised to false --> overload gets confused

		void SetPaused(std::string animatorId, bool value = true);

		bool IsPlayingAnimation(std::string animatorId, std::string animationId) const;

		bool IsPlayingAnimation(std::string animatorId) const;

		bool IsPlayingAnimation() const;

		// Retuns true if the animator is currently playing the animation with animation id = animationId and is paused, false otherwise
		bool IsPaused(std::string animatorId, std::string animationId) const;

		bool IsPaused(std::string animatorId) const;

		// Returns a list of normalised strings
		std::vector<std::string> GetPlayingAnimations() const;

	private:
		AnimatorDictionary animatorDictionary;
	};

} // namespace mbe