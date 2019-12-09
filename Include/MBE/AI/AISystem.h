#pragma once

/// @file
/// @brief Classes mbe::BaseAISystem

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

} // namespace mbe