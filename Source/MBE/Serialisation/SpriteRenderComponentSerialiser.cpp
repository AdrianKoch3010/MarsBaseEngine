#include <MBE/Graphics/SpriteRenderComponent.h>
#include <MBE/Core/Utility.h>

#include <MBE/Serialisation/SpriteRenderComponentSerialiser.h>

using namespace mbe;

void SpriteRenderComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	auto& spriteRenderComponent = entity.AddComponent<SpriteRenderComponent>();

	const auto colourElement = componentData.FirstChildElement("Colour");
	if (colourElement != nullptr)
	{
		auto colour = ColourSerialiser::Load(*colourElement);
		spriteRenderComponent.SetColor(colour);
	}
}

void SpriteRenderComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have a transform component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<SpriteRenderComponent>() == false)
		throw std::runtime_error("Store sprite render component: The entity must have an mbe::SpriteRenderComponent");

	const auto& spriteRenderComponent = entity.GetComponent<SpriteRenderComponent>();

	auto colourElement = document.NewElement("Colour");
	ColourSerialiser::Store(spriteRenderComponent.GetColor(), document, *colourElement);
	componentData.InsertEndChild(colourElement);
}
