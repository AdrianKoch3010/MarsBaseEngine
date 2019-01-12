#pragma once

/// @file
/// @brief Class mbe::RenderInformationComponent

#include <functional>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Core/Entity.h>

namespace mbe
{

	class RenderInformationComponent : public Component
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
		typedef std::shared_ptr<RenderInformationComponent> Ptr;
		typedef std::weak_ptr<RenderInformationComponent> WPtr;
		typedef std::unique_ptr<RenderInformationComponent> UPtr;

		// A pointer is returned since the value may be null if this entity is not been rendered
		typedef std::function<const sf::View *(const Entity &)> ViewGetterFunction;
		typedef std::function<const sf::RenderWindow *()> WindowGetterFunction;

	public:
		RenderInformationComponent(Entity & parentEntity, RenderLayer renderLayer = RenderLayer::GUI, float zOrder = 0.f);
		~RenderInformationComponent() = default;

	public:
		inline void SetRenderLayer(RenderLayer renderLayer) { this->renderLayer = renderLayer; }

		inline void SetZOrder(float value) { zOrder = value; }

		inline void SetViewGetterFunction(ViewGetterFunction function) { viewGetterFunction = function; }

		inline void SetWindowGetterFunction(WindowGetterFunction function) { windowGetterFunction = function; }

		inline RenderLayer GetRenderLayer() const { return renderLayer; }

		inline float GetZOrder() const { return zOrder; }

		const sf::View * GetView() const;

		const sf::RenderWindow * GetRenderWindow() const;

		void ResetViewGetterFunction();

		void ResetWindowGetterFunction();

		// Returns true if the passed in render information component would be drawn above this
		bool IsAboveOrEqual(const RenderInformationComponent & renderInformationComponent) const;

	public:
		// Returns true if is a is above or equal b
		static bool IsAboveOrEqual(const RenderInformationComponent & a, const RenderInformationComponent & b);

	private:
		RenderLayer renderLayer;
		float zOrder;
		ViewGetterFunction viewGetterFunction;
		WindowGetterFunction windowGetterFunction;
	};

	using RenderLayer = RenderInformationComponent::RenderLayer;

	RenderLayer operator++ (RenderLayer & renderLayer);

	bool operator == (RenderLayer a, RenderLayer b);

	bool operator != (RenderLayer a, RenderLayer b);

	bool operator > (RenderLayer a, RenderLayer b);

	bool operator < (RenderLayer a, RenderLayer b);

	bool operator >= (RenderLayer a, RenderLayer b);

	bool operator <= (RenderLayer a, RenderLayer b);

} // namespace mbe