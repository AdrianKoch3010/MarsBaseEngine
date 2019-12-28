#include <string>

#include <MBE/Serialisation/AnimationComponentSerialser.h>

using namespace mbe;

void AnimationComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	using namespace tinyxml2;

	// Load the list of entities
	for (auto animatorElement = componentData.FirstChildElement("Animator"); animatorElement != nullptr; animatorElement = animatorElement->NextSiblingElement("Animator"))
	{
		// Get the animator id
		auto animatorId = animatorElement->Attribute("id");
		if (animatorId == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator id");
		std::string animatorIdString{ animatorId };

		bool active, loop, paused;
		float progress;
		
		// Get Active
		const auto activeElement = animatorElement->FirstChildElement("Active");
		if (activeElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator active element");
		if (activeElement->QueryBoolText(&active) != XML_SUCCESS)
			throw std::runtime_error("Load animation component: Failed to parse animator active text");

		// Get Loop
		const auto loopElement = animatorElement->FirstChildElement("Loop");
		if (loopElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator loop element");
		if (loopElement->QueryBoolText(&loop) != XML_SUCCESS)
			throw std::runtime_error("Load animation component: Failed to parse animator loop text");

		// Get Paused
		const auto pausedElement = animatorElement->FirstChildElement("Paused");
		if (pausedElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator paused element");
		if (pausedElement->QueryBoolText(&paused) != XML_SUCCESS)
			throw std::runtime_error("Load animation component: Failed to parse animator paused text");

		// Get Progress
		const auto progressElement = animatorElement->FirstChildElement("Progress");
		if (progressElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator progress element");
		if (progressElement->QueryFloatText(&progress) != XML_SUCCESS)
			throw std::runtime_error("Load animation component: Failed to parse animator progress text");


		// Load recreate the amimations
		auto animationsElement = animatorElement->FirstChildElement("Animations");
		if (animationsElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator animations element");

		//for (auto animationElement = animationsElement->FirstChildElement("Animation"); animationElement != nullptr; animationElement = animationElement->NextSiblingElement("Animation"))
		//{
		//	// Get the animation type
		//	auto animationType = animationElement->Attribute("type");
		//	if (animationType == nullptr)
		//		throw std::runtime_error("Load Components: Failed to parse component type");
		//	std::string componentTypeString{ animationType };

		//	// Call the appropreate animation serialiser for that animation type
		//	if (componentSerialiserDictionary.find(componentTypeString) == componentSerialiserDictionary.end())
		//		throw std::runtime_error("Load Components: No component serialiser found for this type");
		//	componentSerialiserDictionary.at(componentTypeString)->LoadComponent(entity, *animationElement);
		//}
	}
}

void AnimationComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	// Store the animators
}
