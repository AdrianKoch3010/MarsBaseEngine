#include <exception>

#include <MBE/TransformComponent.h>

#include <MBE/Serialisation/TransformComponentSerialiser.h>

using namespace mbe;

void TransformComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	// The default values are left to the transform component
	sf::Vector2f position, origin, scale;
	float rotation = 0;

	auto& transformComponent = entity.AddComponent<TransformComponent>();

	// Get position
	const auto positionElement = componentData.FirstChildElement("Position");
	if (positionElement != nullptr)
	{
		if (positionElement->QueryFloatAttribute("x", &position.x) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to pass Position x attribute", positionElement->GetLineNum());
		if (positionElement->QueryFloatAttribute("y", &position.y) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Position y attribute", positionElement->GetLineNum());
		transformComponent.SetPosition(position);
	}

	// Get origin
	const auto originElement = componentData.FirstChildElement("Origin");
	if (originElement != nullptr)
	{
		if (originElement->QueryFloatAttribute("x", &origin.x) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Origin x attribute", originElement->GetLineNum());
		if (originElement->QueryFloatAttribute("y", &origin.y) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Origin y attribute", originElement->GetLineNum());
		transformComponent.SetOrigin(origin);
	}

	// Get scale
	const auto scaleElement = componentData.FirstChildElement("Scale");
	if (scaleElement != nullptr)
	{
		if (scaleElement->QueryFloatAttribute("x", &scale.x) != XML_SUCCESS)
		throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Scale x attribute", scaleElement->GetLineNum());

		if (scaleElement->QueryFloatAttribute("y", &scale.y) != XML_SUCCESS)
		throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Scale y attribute", scaleElement->GetLineNum());

		transformComponent.SetScale(scale);
	}

	// Get rotation
	const auto rotationElement = componentData.FirstChildElement("Rotation");
	if (rotationElement != nullptr)
	{
		if (rotationElement->QueryFloatText(&rotation) != XML_SUCCESS)
		throw ParseError(MBE_NAME_OF(TransformComponentSerialiser), "Failed to parse Rotation text", rotationElement->GetLineNum());

		transformComponent.SetRotation(rotation);
	}
}

void TransformComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	// The entity must have a transform component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<TransformComponent>() == false)
		throw std::runtime_error("Store transform component: The entity must have an mbe::TransfromComponent");

	const auto& transformComponent = entity.GetComponent<TransformComponent>();
	

	// Store Position
	auto positionElement = document.NewElement("Position");
	positionElement->SetAttribute("x", transformComponent.GetPosition().x);
	positionElement->SetAttribute("y", transformComponent.GetPosition().y);
	componentData.InsertEndChild(positionElement);

	// Store Origin
	auto originElement = document.NewElement("Origin");
	originElement->SetAttribute("x", transformComponent.GetOrigin().x);
	originElement->SetAttribute("y", transformComponent.GetOrigin().y);
	componentData.InsertEndChild(originElement);

	// Store Scale
	auto scaleElement = document.NewElement("Scale");
	scaleElement->SetAttribute("x", transformComponent.GetScale().x);
	scaleElement->SetAttribute("y", transformComponent.GetScale().y);
	componentData.InsertEndChild(scaleElement);

	// Store Rotation
	auto rotationElement = document.NewElement("Rotation");
	rotationElement->SetText(transformComponent.GetRotation());
	componentData.InsertEndChild(rotationElement);
}
