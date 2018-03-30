#include <MBE/Graphics/RenderManager.h>

using namespace mbe;

RenderManager::RenderManager()
{
}

void RenderManager::Render(sf::RenderTarget & renderTarget)
{
	// Remove all expired nodes
	this->Refresh();

	for (auto renderLayer = RenderObject::RenderLayer::Background; renderLayer != RenderObject::RenderLayer::LayerCount; ++renderLayer)
	{
		// Draw all the render nodes in the current layer
		renderTarget.setView(viewDictionary[renderLayer]);
		auto & renderObjectIdList = renderObjectDictionary[renderLayer];

		// Sort the render nodes based on their z-order
		SortByZOrder(renderObjectIdList);

		for (const auto renderObjectId : renderObjectIdList)
		{
			// Get the pointer that corresponds to the id
			auto & renderObject = *RenderObject::GetObjectFromID(renderObjectId);

			// Culling - only draw if the entity is visible on screen
			if (this->IsVisible(renderObject))
			{
				// If the entity still exists
				// This should always be the case since expired entities are removed in the refresh function
				renderObject.Draw(renderTarget);
			}
		}
	}
}

void RenderManager::AddRenderObject(const RenderObject & renderObject)
{
	renderObjectDictionary[renderObject.GetRenderLayer()].push_back(renderObject.GetHandleID());
}

void RenderManager::AddRenderObject(RenderObject::HandleID renderDataId)
{
	assert(RenderObject::GetObjectFromID(renderDataId) != nullptr && "RenderManager: The render data must exist");

	auto & renderData = *RenderObject::GetObjectFromID(renderDataId);
	renderObjectDictionary[renderData.GetRenderLayer()].push_back(renderDataId);
}

void RenderManager::RemoveRenderObject(const RenderObject & renderObject)
{
	auto renderObjectIdList = renderObjectDictionary[renderObject.GetRenderLayer()];

	auto it = std::find(renderObjectIdList.cbegin(), renderObjectIdList.cend(), renderObject.GetHandleID());
	if (it == renderObjectIdList.cend())
		throw std::runtime_error("RenderManager: The render object has npt been added");

	renderObjectIdList.erase(it);
}

void RenderManager::RemoveRenderObject(RenderObject::HandleID renderObjectId)
{
	assert(RenderObject::GetObjectFromID(renderObjectId) != nullptr && "RenderManager: The render data must exist");

	auto renderLayer = RenderObject::GetObjectFromID(renderObjectId)->GetRenderLayer();
	auto & renderObjectIdList = renderObjectDictionary[renderLayer];

	auto it = std::find(renderObjectIdList.cbegin(), renderObjectIdList.cend(), renderObjectId);
	if (it == renderObjectIdList.cend())
		throw std::runtime_error("RenderManager: The render data has not been added");

	renderObjectIdList.erase(it);
}

void RenderManager::SetView(RenderObject::RenderLayer renderLayer, const sf::View & view)
{
	viewDictionary[renderLayer] = view;
}

sf::View & RenderManager::GetView(RenderObject::RenderLayer renderLayer)
{
	return viewDictionary[renderLayer];
}

const sf::View & RenderManager::GetView(RenderObject::RenderLayer renderLayer) const
{
	return viewDictionary[renderLayer];
}

bool RenderManager::IsVisible(const RenderObject & renderObject) const
{
	const auto renderLayer = renderObject.GetRenderLayer();
	const auto & view = viewDictionary[renderLayer];
	const auto & boundingBox = renderObject.GetBoundingBox();

	// Calculate whether the object can be seen
	return true;
}

void RenderManager::Refresh()
{
	// Remove expired render data objects
	// Entities are automatically deleted when they expire (This is much more efficient than calling RemoveRenderEntity())
	for (auto & pair : renderObjectDictionary)
	{
		auto & renderObjectIdList = pair.second;
		for (auto it = renderObjectIdList.cbegin(); it != renderObjectIdList.cend(); )
		{
			if (RenderObject::GetObjectFromID(*it))
				it = renderObjectIdList.erase(it);
			else
				++it;
		}
	}
}

void RenderManager::SortByZOrder(std::vector<RenderObject::HandleID> & renderObjectIdList)
{
	// Use insertion sort to sort by zOrder
	// Insertion sort is fast for nearly sorted lists
	// This will nearly always be the case since not many, if any object will changes between two frames
	// Furthermore, it is a consistent sort which prevents flickering if two render nodes have the z-order
	size_t size = renderObjectIdList.size();
	for (size_t i = 1; i < size; i++)
	{
		for (size_t j = i; j > 0 && RenderObject::GetObjectFromID(renderObjectIdList[j - 1])->GetZOrder() > RenderObject::GetObjectFromID(renderObjectIdList[j])->GetZOrder(); j--)
		{
			std::swap(renderObjectIdList[j], renderObjectIdList[j - 1]);
		}
	}
}
