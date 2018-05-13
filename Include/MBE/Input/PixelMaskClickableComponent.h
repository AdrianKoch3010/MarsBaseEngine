#pragma once

/// @file
/// @brief Class mbe::PixelMaskClickableComponent

#include <memory>
#include <cassert>

#include <MBE/Input/ClickableComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/Core/PixelMask.h>

namespace mbe
{

	class PixelMaskClickableComponent : public ClickableComponent
	{
	public:
		typedef std::shared_ptr<PixelMaskClickableComponent> Ptr;
		typedef std::weak_ptr<PixelMaskClickableComponent> WPtr;
		typedef std::unique_ptr<PixelMaskClickableComponent> UPtr;

	public:
		PixelMaskClickableComponent(Entity & parentEntity);
		~PixelMaskClickableComponent() = default;

	public:
		// In local coordinates
		bool Contains(const sf::Vector2f & position) const override;

	};

} // namespace mbe