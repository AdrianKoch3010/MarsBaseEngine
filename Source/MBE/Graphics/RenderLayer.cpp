#include <MBE/Graphics/RenderLayer.h>

using mbe::Entity;
using mbe::detail::RenderLayer;

void RenderLayer::AddRenderEntity(Entity::HandleID entityId)
{
	renderEntityIdList.push_back(entityId);
}

void RenderLayer::RemoveRenderEntity(Entity::HandleID entityId)
{
	// std::find_if returns an it to the first intance for which the lambda returns true
	auto it = std::find_if(renderEntityIdList.begin(), renderEntityIdList.end(), [=](auto currentEntityId)
	{
		return entityId == currentEntityId;
	});

	if (it == renderEntityIdList.end())
		throw std::runtime_error("The render node could not be found");

	renderEntityIdList.erase(it);
}
