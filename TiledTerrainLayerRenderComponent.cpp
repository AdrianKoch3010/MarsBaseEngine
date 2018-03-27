#include "TiledTerrainLayerRenderComponent.h"

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(TiledTerrainLayerRenderComponent, RenderComponent)

TiledTerrainLayerRenderComponent::TiledTerrainLayerRenderComponent(Entity & parentEntity, const sf::Texture & tilesetTexture) :
	RenderComponent(parentEntity, RenderLayer::Foreground),
	tilesetTexture(tilesetTexture),
	isCreated(false)
{
	assert(parentEntity.HasComponent<TransformComponent>() && "TiledTerrainLayerRenderComponent: The parent entity must have a mbe::TransformComponent");
}

void TiledTerrainLayerRenderComponent::Draw(sf::RenderTarget & target)
{
	// Only draw the vertices if the tiled terrain layer has been created
	if (this->IsCreated() == false)
		return;

	sf::RenderStates states;
	states.texture = &tilesetTexture;
	states.transform = parentEntity.GetComponent<TransformComponent>().GetWorldTransform();

	target.draw(vertices, states);
}

sf::FloatRect TiledTerrainLayerRenderComponent::GetBoundingBox() const
{
	sf::FloatRect rect;
	const auto & transformComponent = parentEntity.GetComponent<TransformComponent>();
	const auto & tiledTerrainLayerComponent = parentEntity.GetComponent<TiledTerrainLayerComponent>();

	rect.left = transformComponent.GetPosition().x;
	rect.top = transformComponent.GetPosition().y;
	rect.width = static_cast<float>(tiledTerrainLayerComponent.GetSize().x * tiledTerrainLayerComponent.GetTileSize().x);
	rect.height = static_cast<float>(tiledTerrainLayerComponent.GetSize().y * tiledTerrainLayerComponent.GetTileSize().y);

	return rect;	
}