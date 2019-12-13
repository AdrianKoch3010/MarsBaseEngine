#pragma once

/// @file
/// @brief Classes mbe::BaseAISystem

#include <memory>

#include <MBE/Core/EntityManager.h>
#include <MBE/Core/EventManager.h>
#include <MBE/AI/AIComponent.h>

namespace mbe
{

	class BaseAISystem
	{
	public:
		typedef std::shared_ptr<BaseAISystem> Ptr;
		typedef std::weak_ptr<BaseAISystem> WPtr;
		typedef std::unique_ptr<BaseAISystem> UPtr;

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