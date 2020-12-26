#pragma once

/// @file
/// @brief class mbe::AnimationSystem

#include <memory>

#include <SFML/System/Time.hpp>

#include <MBE/Core/EntityManager.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Animation/AnimationHolder.h>

namespace mbe
{

	/// @brief Updates the animators and animations of the animation components
	class AnimationSystem
	{
	public:
		typedef std::shared_ptr<AnimationSystem> Ptr;
		typedef std::weak_ptr<AnimationSystem> WPtr;
		typedef std::unique_ptr<AnimationSystem> UPtr;

	public:
		/// @brief Constructor
		/// @param entityManager A reference to the mbe::EntityManager whos entities will be updated
		/// @param animationHolder A reference to the mbe::AnimationHolder managing the global animation templates
		AnimationSystem(EntityManager& entityManager, AnimationHolder& animationHolder);

		/// @brief Default destructor
		~AnimationSystem() = default;

	public:
		/// @Acts on all entities with an mbe::AnimationComponent
		/// @details This function should be called once per frame. The function iterates over the animators of an entity
		/// and updates the currently playing animations.
		/// @param frameTime The time that passed between two consecutive calls
		void Update(sf::Time frameTime);

	private:
		EntityManager& entityManager;
		AnimationHolder& animationHolder;
	};


} // namespace mbe