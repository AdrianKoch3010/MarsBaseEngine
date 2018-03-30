#pragma once

/// @file
/// @brief Class mbe::RenderManager

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

#include <MBE/Graphics/RenderObject.h>


// In order for something to get drawn:
// - RenderLayer
// - zOrder
// - Draw function
// - Cull function

namespace mbe
{

	class RenderManager
	{
	private:
		// When using the std::unordered_map operator[] the a default sf::View instance is created
		typedef std::unordered_map<RenderObject::RenderLayer, sf::View> ViewDictionary;
		// When using the std::unordered_map operator[] an emtpty std::vector<RenderObject::HandleID>() is created
		typedef std::unordered_map<RenderObject::RenderLayer, std::vector</*typename */RenderObject::HandleID>> RenderDataDictionary;

	public:
		RenderManager();
		~RenderManager() = default;

	public:
		void Render(sf::RenderTarget & renderTarget);

		void AddRenderObject(const RenderObject & renderObject);

		void AddRenderObject(RenderObject::HandleID renderObjectId);

		// Throws if the render object could not be found
		void RemoveRenderObject(const RenderObject & renderObject);
		
		// Throws if the render object could not be found
		void RemoveRenderObject(RenderObject::HandleID renderObjectId);

		void SetView(RenderObject::RenderLayer renderLayer, const sf::View & view);
		// This function should be prefered over SetView() since the existsing view can be modified
		sf::View & GetView(RenderObject::RenderLayer renderLayer);
		// Const overload
		const sf::View & GetView(RenderObject::RenderLayer renderLayer) const;

		bool IsVisible(const RenderObject & renderObject) const;

	private:
		// Removes expires render data instances
		void Refresh();

		// Performs an insertion sort
		// Assumes that render nodes exist for all render node ids
		static void SortByZOrder(std::vector<RenderObject::HandleID> & renderObjectIdList);

	private:
		mutable ViewDictionary viewDictionary;
		RenderDataDictionary renderObjectDictionary;
	};

} // namespace mbe