#pragma once

/// @file
/// @brief Class mbe::SpriteRenderSystem

#include <cassert>

#include <MBE/Graphics/BaseComponentRenderSystem.h>

#include <MBE/Graphics/SpriteRenderComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/TransformComponent.h>

namespace mbe
{

	class SpriteRenderSystem : public BaseComponentRenderSystem
	{
	public:
		typedef std::shared_ptr<SpriteRenderSystem> Ptr;
		typedef std::weak_ptr<SpriteRenderSystem> WPtr;
		typedef std::unique_ptr<SpriteRenderSystem> UPtr;

	public:
		SpriteRenderSystem(const EntityManager & entityManager);
		~SpriteRenderSystem() = default;

	public:
		void Update() override;
	};

} // namespace mbe