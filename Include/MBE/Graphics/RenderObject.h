#pragma once

/// @file
/// @brief Class mbe::RenderObject

#include <type_traits>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <MBE/Core/HandleBase.h>

namespace mbe
{
	class RenderObject : public HandleBase<RenderObject>
	{
	public:
		enum class RenderLayer : unsigned short int
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
		RenderObject(RenderLayer renderLayer = RenderLayer::GUI, float zOrder = 0.f);
		~RenderObject() = default;

	public:
		virtual void Draw(sf::RenderTarget & renderTarget) const = 0;

		virtual sf::FloatRect GetBoundingBox() const = 0;

		inline void SetZOrder(float value) { zOrder = value; }

		inline RenderLayer GetRenderLayer() const { return renderLayer; }

		inline float GetZOrder() const { return zOrder; }

	private:
		RenderLayer renderLayer;
		float zOrder;
	};

	RenderObject::RenderLayer operator++ (RenderObject::RenderLayer & renderLayer);

} // namespace mbe