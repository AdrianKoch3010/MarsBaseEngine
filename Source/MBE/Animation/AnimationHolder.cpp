#include <MBE/Animation/AnimationHolder.h>

#include <fstream>
#include <iostream>

#include <MBE/Core/Utility.h>
#include <MBE/Serialisation/SerialiserRegistry.h>
#include <MBE/Core/Exceptions.h>


using namespace mbe;
using namespace tinyxml2;


void AnimationHolder::Load(const std::string file)
{
	// Look for the object file under that name
	std::ifstream animationFile(file);
	if (animationFile.is_open() == false)
		throw std::runtime_error("AnimationHolder: Failed to load animation file (" + file + ")");
	std::string animationFileString;

	animationFile.seekg(0, std::ios::end);
	animationFileString.reserve(animationFile.tellg());
	animationFile.seekg(0, std::ios::beg);

	animationFileString.assign((std::istreambuf_iterator<char>(animationFile)), std::istreambuf_iterator<char>());

	// Convert to XML element
	XMLDocument document;
	XMLError error = document.Parse(animationFileString.c_str());

	if (error != XML_SUCCESS)
		throw std::runtime_error("AnimationHolder: The animation file (" + file + ") could be parsed");

	const auto rootNode = document.FirstChildElement();
	if (rootNode == nullptr)
		throw ParseError(MBE_NAME_OF(AnimationHolder), "The root node could not be found", document.GetLineNum());

	// Iterate over the animations
	for (auto animationElement = rootNode->FirstChildElement("Animation"); animationElement != nullptr; animationElement = animationElement->NextSiblingElement("Animation"))
	{
		// Get the animation type
		auto animationType = animationElement->Attribute("type");
		if (animationType == nullptr)
			throw ParseError(MBE_NAME_OF(AnimationHolder), "Failed to parse Animation type attribute", animationElement->GetLineNum());
		std::string animationTypeString{ animationType };

		// Get the animation id
		auto animationId = animationElement->Attribute("globalId");
		if (animationId == nullptr)
			throw ParseError(MBE_NAME_OF(AnimationHolder), "Failed to parse Animation global id attribute", animationElement->GetLineNum());
		std::string animationIdString{ animationId };
		NormaliseIDString(animationIdString);

		try
		{
			animationDictionary.insert({ animationIdString, AnimationSerialiserRegistry::Instance()[animationTypeString].Parse(*animationElement) });
		}
		catch (const ParseError& parseError)
		{
			throw parseError;
		}
		catch (std::runtime_error& error)
		{
			throw ParseError(MBE_NAME_OF(AnimationHolder), "Unknown animation serialiser (" + animationTypeString + ")", animationElement->GetLineNum());
		}
	}
}


const EntityAnimator::AnimationFunction& AnimationHolder::GetAnimation(std::string animationId) const
{
	NormaliseIDString(animationId);

	auto it = animationDictionary.find(animationId);
	if (it == animationDictionary.cend())
		throw std::runtime_error("AnimationHolder: No animation exists under this id: " + animationId);

	return it->second;
}