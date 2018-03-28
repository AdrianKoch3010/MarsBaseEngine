#pragma once

/// @file
/// @brief Class mbe::RenderComponent

#include <SFML/Graphics.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{
	class RenderComponent : public Component
	{
	public:
		/// @brief The render layers used to determine which objects are drawn after each other
		/// @details The background is drawn first followed by the foreground and all the objects. Lastly,
		/// the gui is drawn. Note that object later drawn are drawn above previously drawn objects.
		enum RenderLayer
		{
			/// The background such as a stationary picture or parrallax scrolling
			Background,
			/// The forground such as a tile map or platforms for a platformer
			Foreground,
			/// All objects such as movable items, characters etc.
			Objects,
			/// The graphical user interface
			GUI,
			/// The number of the render layers
			LayerCount
		};

	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		/// @param eventManager A reference the mbe::EventManager that is used to raise the events:
		/// @n - RenderComponentCreatedEvent
		/// @n - RenderComponentRemovedEvent
		/// @param texture The texture of the RenderComponent
		RenderComponent(Entity & parentEntity, RenderLayer renderLayer);

		/// @brief Default destructor
		virtual ~RenderComponent() = default;

	public:
		// Maybe implement the same functionality using an entity moved event
		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
		/// @param frameTime The delta time between two successive frames
		inline void Update(sf::Time frameTime) override {};

		virtual void Draw(sf::RenderTarget & target) = 0;

		// No reference is returned since the bounding box may be calculated within the function - avoid local references
		virtual sf::FloatRect GetBoundingBox() const = 0; // can be determined by the inherited class

		inline void SetZOrder(float value) { zOrder = value; }

		inline auto GetZOrder() const { return zOrder; }

		inline RenderLayer GetRenderLayer() const { return renderLayer; }

	private:
		float zOrder;
		const RenderLayer renderLayer;
	};
} // namespace mbe

