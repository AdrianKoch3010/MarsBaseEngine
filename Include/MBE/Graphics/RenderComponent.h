#pragma once

/// @file
/// @brief Class mbe::RenderComponent

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>
#include <MBE/Graphics/RenderObject.h>

namespace mbe
{
	class RenderComponent : public Component, public RenderObject
	{
	//private:
	//	// Calls the respective Draw() and IsVisible() methods of this class
	//	class DrawableAdapter : public Drawable
	//	{
	//	public:
	//		DrawableAdapter(const RenderComponent & renderComponent) : renderComponent(renderComponent) {}

	//	public:
	//		inline void Draw(sf::RenderTarget & target) const override { renderComponent.Draw(target); }

	//		inline bool IsVisible(const sf::View & view) const override { return renderComponent.IsVisible(view); }

	//	private:
	//		const RenderComponent & renderComponent;
	//	};

	public:
		/// @brief Constructor
		/// @param parentEntity A reference to the mbe::Entity in which this class is created (To which it then belongs to)
		/// @param renderLayer The renderLayer in which this render object will be drawn
		RenderComponent(Entity & parentEntity, RenderObject::RenderLayer renderLayer);

		/// @brief Default destructor
		virtual ~RenderComponent() = default;

	public:
		/// @brief The update method that should be called every frame (automatically done by the mbe::EntityManager)
		/// @param frameTime The delta time between two successive frames
		inline void Update(sf::Time frameTime) override {};

		//virtual void Draw(sf::RenderTarget & target) const = 0;

		//virtual bool IsVisible(const sf::View & view) const = 0;

		//inline void SetZOrder(float value) { zOrder = value; }

		//inline auto GetZOrder() const { return zOrder; }

		//inline auto GetRenderLayer() const { return renderLayer; }

		//inline const Drawable & GetDrawable() const { return drawableAdapter; }

	private:

		//DrawableAdapter drawableAdapter;
		//float zOrder;
		//const RenderObject::RenderLayer renderLayer;
	};
} // namespace mbe

