#pragma once

/// @file
/// @brief class mbe::AnimationSystem

#include <memory>

#include <SFML/System/Time.hpp>

#include <MBE/Core/EntityManager.h>
#include <MBE/Animation/AnimationComponent.h>

namespace mbe
{

	class AnimationSystem
	{
	public:
		typedef std::shared_ptr<AnimationSystem> Ptr;
		typedef std::weak_ptr<AnimationSystem> WPtr;
		typedef std::unique_ptr<AnimationSystem> UPtr;

	public:
		AnimationSystem(EntityManager& entityManager);
		~AnimationSystem() = default;

	public:
		void Update(sf::Time frameTime);

	private:
		EntityManager& entityManager
	};


} // namespace mbe