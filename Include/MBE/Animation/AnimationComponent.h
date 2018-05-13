//#pragma once
//
///// @file
///// @brief Class mbe::AnimationComponent
//
//#include <string>
//#include <algorithm>
//#include <unordered_map>
//#include <vector>
//#include <limits>
//
//#include <MBE/Animation/Animator.h>
//#include <MBE/Core/Component.h>
//#include <MBE/Core/Utility.h>
//
//namespace mbe
//{
//	//namespace detail
//	//{
//	//	typedef std::size_t AnimatorTypeID;
//
//	//	/// @brief Returns a unique number (for each function call) of type std::size_t
//	//	inline AnimatorTypeID GetAnimatorID() noexcept
//	//	{
//	//		// This will only be initialised once
//	//		static AnimatorTypeID lastId = 0;
//
//	//		// After the first initialisation a new number will be returned for every function call
//	//		return lastId++;
//	//	}
//
//	//	/// @brief Returns a unique number (of type std::size_t) for each type T
//	//	/// @details Each component type will have its own unique id.
//	//	/// The id will be the same for every instance of that type
//	//	/// @tparam T The type for which the id is generated
//	//	template <typename T>
//	//	inline AnimatorTypeID GetAnimatorTypeID() noexcept
//	//	{
//	//		// There will be only one static variable for each template type
//	//		static AnimatorTypeID typeId = GetAnimatorID();
//	//		return typeId;
//	//	}
//	//}
//
//	// ATM animations can only be added at the beginning or when adding a new animator
//	// Use handle base for the animator?
//	// Add the template id type (replace std::string with TID)
//
//	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
//	/// Further they should only use <b>ASCII</b> characters
//	class AnimationComponent : public Component
//	{
//	private:
//		typedef std::map<std::string, BaseAnimator<>::Ptr> BaseAnimatorDictionary;
//
//	public:
//		/// @brief Constructor
//		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
//		AnimationComponent(Entity & parentEntity);
//
//		/// @brief Default destructor
//		~AnimationComponent() = default; // Delete the BaseAnimators
//
//	public:
//		// Add the templated id later on
//		/// @brief Creates an animator and adds it to the list of animators managed by this component
//		/// @tparam TAnimated The type of object being animated. This will determine the type of the animator that is created
//		/// and the type of the animation functions that can be added to it.
//		/// @param id The id of the animator that will later be used to acess it
//		/// @param animatedObject A reference to the object that the created animator will animate. It must inherit from mbe::HandleBase.
//		/// If the animated object gets destroyed, the animator will be deleted. It is generally good practice to animate only objects that are part of this entity
//		/// @returns A reference of the animator created.
//		/// @attention When the animated object gets deleted the animator animating it is deleted as well. Hence, references of the animator may become invalid.
//		/// For this reason animations should only be added when creating the animator. If for some reason a reference must be kept, use Animator<TAnimated>::GetHandleID().
//		template<class TAnimated>
//		Animator<TAnimated> & CreateAnimator(std::string id, TAnimated & animatedObject);
//
//		/// @brief Searches for an animator with the given id
//		/// @details This function exists purely for convenience and should not be needed. To play, pause, stop or check whether an animation of a specific animator
//		/// is playing or paused use the respective overloads of the PlayAnimation(), StopAnimation(), SetPaused() and IsPlayingAnimation() and IsPaused() methods.
//		/// @param The id of the animator to get
//		/// @returns A reference of the requested animator.
//		/// @throws if no animator could be found
//		const BaseAnimator<> & GetAnimator(std::string id) const;
//
//		//template <class TAnimated>
//		//const Animator<TAnimated>::HandleID GetAnimator(std::string id) const;
//
//		// Throws
//		// See: https://stackoverflow.com/questions/18463937/how-to-prevent-implicit-conversion-from-char-array-to-bool
//		// Carefull to not call PlayAnimation(string, string) --> this will call PlayAnimation(string, bool)!!!!!
//		void PlayAnimation(std::string animatorId, const std::string & animationId, bool loop); // Loop can't be default initialised to false --> overload gets confused
//
//		// Throws
//		// Gets every animator that has the requested animation and plays it
//		// This can be used to either play an animation when the animator id is not known or when playing multiple animations with the same id (of different animators)
//		void PlayAnimation(const std::string & animationId, bool loop = false);
//		
//		void StopAnimation(std::string animatorId, std::string animationId);
//
//		void StopAnimation(std::string animationId);
//
//		void StopAnimation();
//
//		void SetPaused(std::string animatorId, std::string animationId, bool value); // Loop can't be default initialised to false --> overload gets confused
//
//		void SetPaused(std::string animatorId, bool value = true);
//
//		bool IsPlayingAnimation(std::string animatorId, std::string animationId) const;
//
//		bool IsPlayingAnimation(std::string animatorId) const;
//
//		bool IsPlayingAnimation() const;
//
//		// Retuns true if the animator is currently playing the animation with animation id = animationId and is paused, false otherwise
//		bool IsPaused(std::string animatorId, std::string animationId) const;
//
//		bool IsPaused(std::string animatorId) const;
//
//		// Returns string in lowercase
//		std::vector<std::string> GetPlayingAnimations() const;
//
//		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
//		/// @param frameTime The delta time between two successive frames
//		void Update(sf::Time frameTime);
//
//	private:
//		BaseAnimatorDictionary animatorDictionary;
//	};
//
//#pragma region Template Implementations
//
//	template<class TAnimated>
//	inline Animator<TAnimated>& AnimationComponent::CreateAnimator(std::string id, TAnimated & animatedObject)
//	{
//		// Convert id string to lower case
//		NormaliseIDString(id);
//
//		auto animatorPtr = std::make_unique<Animator<TAnimated>>(animatedObject);
//		auto & animator = *animatorPtr;
//		animatorDictionary.insert(std::make_pair(id, std::move(animatorPtr)));
//		return animator;
//	}
//
//#pragma endregion
//
//} // namespace mbe

#pragma once

/// @file
/// @brief Class mbe::AnimationComponent

#include <string>
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

		typedef std::map<std::string, EntityAnimator::Ptr> AnimatorDictionary;

	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		AnimationComponent(Entity & parentEntity);

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

		EntityAnimator & GetAnimator(std::string id);

		// Const overload
		/// @brief Searches for an animator with the given id
		/// @details This function exists purely for convenience and should not be needed. To play, pause, stop or check whether an animation of a specific animator
		/// is playing or paused use the respective overloads of the PlayAnimation(), StopAnimation(), SetPaused() and IsPlayingAnimation() and IsPaused() methods.
		/// @param The id of the animator to get
		/// @returns A reference of the requested animator.
		/// @throws if no animator could be found
		const EntityAnimator & GetAnimator(std::string id) const;

		//template <class TAnimated>
		//const Animator<TAnimated>::HandleID GetAnimator(std::string id) const;

		// Throws
		// See: https://stackoverflow.com/questions/18463937/how-to-prevent-implicit-conversion-from-char-array-to-bool
		// Carefull to not call PlayAnimation(string, string) --> this will call PlayAnimation(string, bool)!!!!!
		void PlayAnimation(std::string animatorId, const std::string & animationId, bool loop); // Loop can't be default initialised to false --> overload gets confused

																								// Throws
																								// Gets every animator that has the requested animation and plays it
																								// This can be used to either play an animation when the animator id is not known or when playing multiple animations with the same id (of different animators)
		void PlayAnimation(const std::string & animationId, bool loop = false);

		void StopAnimation(std::string animatorId, std::string animationId);

		void StopAnimation(std::string animationId);

		void StopAnimation();

		void SetPaused(std::string animatorId, std::string animationId, bool value); // Loop can't be default initialised to false --> overload gets confused

		void SetPaused(std::string animatorId, bool value = true);

		bool IsPlayingAnimation(std::string animatorId, std::string animationId) const;

		bool IsPlayingAnimation(std::string animatorId) const;

		bool IsPlayingAnimation() const;

		// Retuns true if the animator is currently playing the animation with animation id = animationId and is paused, false otherwise
		bool IsPaused(std::string animatorId, std::string animationId) const;

		bool IsPaused(std::string animatorId) const;

		// Returns string in lowercase
		std::vector<std::string> GetPlayingAnimations() const;

	private:
		//AnimatorDictionary animatorDictionary;
		std::unordered_map<std::string, std::shared_ptr<Animator<Entity>>> animatorDictionary;
	};

} // namespace mbe