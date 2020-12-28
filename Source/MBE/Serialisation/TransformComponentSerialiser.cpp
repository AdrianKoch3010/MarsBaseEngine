#include <exception>

#include <MBE/TransformComponent.h>

#include <MBE/Serialisation/TransformComponentSerialiser.h>

using namespace mbe;

void TransformComponentSerialser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	sf::Vector2f position, origin, scale;
	float rotation;

	// Get position
	const auto positionElement = componentData.FirstChildElement("Position");
	if (positionElement == nullptr)
		throw std::runtime_error("Load transform component: Failed to parse position");
	if (positionElement->QueryFloatAttribute("x", &position.x) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to pass position x attribute");
	if (positionElement->QueryFloatAttribute("y", &position.y) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse position y attribute");

	// Get origin
	const auto originElement = componentData.FirstChildElement("Origin");
	if (originElement == nullptr)
		throw std::runtime_error("Load transform component: Failed to parse origin");
	if (originElement->QueryFloatAttribute("x", &origin.x) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse origin x attribute");
	if (originElement->QueryFloatAttribute("y", &origin.y) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse origin y attribute");

	// Get scale
	const auto scaleElement = componentData.FirstChildElement("Scale");
	if (scaleElement == nullptr)
		throw std::runtime_error("Load transform component: Failed to parse scale");
	if (scaleElement->QueryFloatAttribute("x", &scale.x) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse scale x attribute");
	if (scaleElement->QueryFloatAttribute("y", &scale.y) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse scale y attribute");

	// Get rotation
	const auto rotationElement = componentData.FirstChildElement("Rotation");
	if (rotationElement == nullptr)
		throw std::runtime_error("Load transform component: Failed to parse rotation");
	if (rotationElement->QueryFloatText(&rotation) != XML_SUCCESS)
		throw std::runtime_error("Load transform component: Failed to parse rotation text");

	// Add the component
	auto& transformComponent = entity.AddComponent<TransformComponent>();
	transformComponent.SetPosition(position);
	transformComponent.SetOrigin(origin);
	transformComponent.SetScale(scale);
	transformComponent.SetRotation(rotation);
}

void TransformComponentSerialser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
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
