#pragma once

/// @file
/// @brief Class mbe::PixelMaskClickableComponent

#include <memory>
#include <cassert>

#include <MBE/Input/ClickableComponent.h>

namespace mbe
{

	/// @brief Provides a method Contains() to check whether the entity has been clicked
	/// @attention The component requires an mbe::TextureWrapperComponent to be added first
	class PixelMaskClickableComponent : public ClickableComponent
	{
	public:
		typedef std::shared_ptr<PixelMaskClickableComponent> Ptr;
		typedef std::weak_ptr<PixelMaskClickableComponent> WPtr;
		typedef std::unique_ptr<PixelMaskClickableComponent> UPtr;

	public:
		/// @brief Constructor
		/// @param eventManager A reference to the mbe::EventManager
		/// @parem parentEntity A reference to the mbe::Entity this component belongs to
		PixelMaskClickableComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief Default destructor
		~PixelMaskClickableComponent() = default;

	public:
		// In local coordinates
		bool Contains(const sf::Vector2f & position) const override;

	};

} // namespace mbe