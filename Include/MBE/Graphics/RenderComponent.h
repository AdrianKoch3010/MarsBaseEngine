#pragma once

/// @file
/// @brief Class mbe::RenderComponent

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>
#include <MBE/TransformComponent.h>

namespace mbe
{
	class RenderComponent : public Component
	{
	public:
		typedef std::shared_ptr<RenderComponent> Ptr;
		typedef std::weak_ptr<RenderComponent> WPtr;
		typedef std::unique_ptr<RenderComponent> UPtr;

	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		/// @param renderLayer The renderLayer in which this render object will be drawn
		explicit RenderComponent(EventManager & eventManager, Entity & parentEntity);

		/// @brief Default destructor
		virtual ~RenderComponent() = default;

	public:
		virtual void Draw(sf::RenderTarget & target) const = 0;

		// Default implementation applies the entity's transform if it has a transform component
		virtual sf::FloatRect GetGlobalBounds() const;

		virtual sf::FloatRect GetLocalBounds() const = 0;

		virtual bool IsVisible(const sf::View & view) const;
	};
} // namespace mbe

