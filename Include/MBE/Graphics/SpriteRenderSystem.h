#pragma once

/// @file
/// @brief Class mbe::SpriteRenderSystem

#include <cassert>

#include <MBE/Core/EntityManager.h>

#include <MBE/Graphics/SpriteRenderComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/TransformComponent.h>

namespace mbe
{

	class SpriteRenderSystem
	{
	public:
		SpriteRenderSystem(const EntityManager & entityManager);
		~SpriteRenderSystem() = default;

	public:
		void Update();

	private:
		const EntityManager & entityManager;
	};

} // namespace mbe