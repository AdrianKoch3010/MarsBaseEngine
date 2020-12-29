#include <MBE/Serialisation/CustomRenderComponentSerialiser.h>

#include <MBE/Graphics/CustomRenderComponent.h>

using namespace mbe;

void CustomRenderComponentSerialiser::LoadComponent(mbe::Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	entity.AddComponent<CustomRenderComponent>();
}

void CustomRenderComponentSerialiser::StoreComponent(const mbe::Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// Nothing to store, knowing that the entity has this component is enough
}
