#include <MBE/Animation/AnimationHolder.h>

#include <fstream>
#include <iostream>
#include <exception>

#include <MBE/Core/Utility.h>
#include <MBE/Serialisation/AnimationSerialiserRegistry.h>


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
		throw std::runtime_error("AnimationHolder: The root node could not be found");

	// Iterate over the animations
	for (auto animationElement = rootNode->FirstChildElement("Animation"); animationElement != nullptr; animationElement = animationElement->NextSiblingElement("Animation"))
	{
		// Get the animation type
		auto animationType = animationElement->Attribute("type");
		if (animationType == nullptr)
			throw std::runtime_error("Load animation: Failed to parse animation type");
		std::string animationTypeString{ animationType };

		// Get the animation id
		auto animationId = animationElement->Attribute("globalId");
		if (animationId == nullptr)
			throw std::runtime_error("Load animation: Failed to parse animation globalId");
		std::string animationIdString{ animationId };
		NormaliseIDString(animationIdString);

		const auto& animationSerialiserDictionary = AnimationSerialiserRegistry::Instance().GetAnimationSerialiserDictionary();
		animationDictionary.insert({ animationIdString, animationSerialiserDictionary.at(animationTypeString)->Parse(*animationElement) });
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