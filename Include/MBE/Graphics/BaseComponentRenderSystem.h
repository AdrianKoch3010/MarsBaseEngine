#pragma once

/// @file
/// @brief Class mbe::BaseComponentRenderSystem

#include <memory>

#include <MBE/Core/EntityManager.h>


namespace mbe
{

	class BaseComponentRenderSystem
	{
	public:
		typedef std::shared_ptr<BaseComponentRenderSystem> Ptr;
		typedef std::weak_ptr<BaseComponentRenderSystem> WPtr;
		typedef std::unique_ptr<BaseComponentRenderSystem> UPtr;

	public:
		BaseComponentRenderSystem(const EntityManager & entityManager);
		virtual ~BaseComponentRenderSystem() = default;

		virtual void Update() = 0;
		
	protected:
		const EntityManager & entityManager;
	};

} // namespace mbe