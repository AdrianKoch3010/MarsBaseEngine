#pragma once

/// @file
/// @brief Classes mbe::AISystem and mbe::BaseAISystem

#include <MBE/Core/EntityManager.h>
#include <MBE/Core/EventManager.h>
#include <MBE/AI/AIComponent.h>

namespace mbe
{

	class BaseAISystem
	{
	public:
		BaseAISystem(EntityManager& entityManager, EventManager& eventManager);
		~BaseAISystem() = default;

	public:
		virtual void Update() = 0;

	protected:
		EntityManager & entityManager;
		EventManager & eventManager;
		
	};

	class AISystem
	{
	public:
		AISystem(EntityManager & entityManager);
		~AISystem() = default;

	public:
		void Update();

	private:
		EntityManager & entityManager;

	};

} // namespace mbe