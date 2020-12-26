#include <MBE/Animation/AnimationHolder.h>

#include <fstream>
#include <iostream>
#include <exception>

#include <MBE/Core/Utility.h>

// Default animations and serialisers
#include <MBE/Animation/FrameAnimation.h>
#include <MBE/Serialisation/FrameAnimationSerialiser.h>
#include <MBE/Animation/RotationAnimation.h>
#include <MBE/Serialisation/RotationAnimationSerialiser.h>
#include <MBE/Animation/BlinkingAnimation.h>
#include <MBE/Serialisation/BlinkingAnimationSerialiser.h>
#include <MBE/Animation/PitchAnimation.h>
#include <MBE/Serialisation/PitchAnimationSerialiser.h>

using namespace mbe;
using namespace tinyxml2;

AnimationHolder::AnimationHolder()
{
	// Add the default animation serialisers
	AddAnimationSerialiser<FrameAnimationSerialiser, FrameAnimation>("FrameAnimation");
	AddAnimationSerialiser<RotationAnimationSerialiser, RotationAnimation>("RotationAnimation");
	AddAnimationSerialiser<BlinkingAnimationSerialiser, BlinkingAnimation>("BlinkingAnimation");
	AddAnimationSerialiser<PitchAnimationSerialiser, PitchAnimation>("PitchAnimation");
}

//void AnimationHolder::Load(const std::string& filePath, std::string fileName)
//{
//	std::ifstream animationsListFile(filePath + fileName);
//
//	if (animationsListFile.is_open() == false)
//	{
//		std::cerr << "AnimationHolder: Could not open the animations file (" << fileName << ")" << std::endl;
//		return;
//	}
//
//	std::string line;
//	while (std::getline(animationsListFile, line))
//	{
//		// Ignore empty lines
//		if (line.empty())
//			continue;
//
//		// Ignore lines that start with #
//		if (line.front() == '#')
//			continue;
//
//		// Look for the object file under that name
//		std::ifstream animationFile(filePath + "Animations/" + line + ".xml");
//		if (animationFile.is_open() == false)
//		{
//			std::cerr << "AnimationHolder: Failed to load animation file (" << line << ".xml)" << std::endl;
//			continue;
//		}
//		std::string animationFileString;
//
//		animationFile.seekg(0, std::ios::end);
//		animationFileString.reserve(animationFile.tellg());
//		animationFile.seekg(0, std::ios::beg);
//
//		animationFileString.assign((std::istreambuf_iterator<char>(animationFile)), std::istreambuf_iterator<char>());
//
//		// Convert to XML element
//		XMLDocument document;
//		XMLError error = document.Parse(animationFileString.c_str());
//
//		if (error != XML_SUCCESS)
//			throw std::runtime_error("AnimationHolder: The animation file (" + filePath + "Animations/" + line + ".xml) could be parsed");
//
//		const auto rootNode = document.FirstChildElement();
//		if (rootNode == nullptr)
//			throw std::runtime_error("AnimationHolder: The root node could not be found");
//
//		// Iterate over the animations
//		for (auto animationElement = rootNode->FirstChildElement("Animation"); animationElement != nullptr; animationElement = animationElement->NextSiblingElement("Animation"))
//		{
//			// Get the animation type
//			auto animationType = animationElement->Attribute("type");
//			if (animationType == nullptr)
//				throw std::runtime_error("Load animation: Failed to parse animation type");
//			std::string animationTypeString{ animationType };
//
//			// Get the animation id
//			auto animationId = animationElement->Attribute("globalId");
//			if (animationId == nullptr)
//				throw std::runtime_error("Load animation: Failed to parse animation globalId");
//			std::string animationIdString{ animationId };
//			NormaliseIDString(animationIdString);
//
//			//auto animationString = std::string(animationElement->GetText());
//
//			// Add to the dictionaries
//			//stringDictionary.insert({ animationIdString, animationString });
//
//			animationDictionary.insert({ animationIdString, animationSerialiserDictionary.at(animationTypeString)->Parse(*animationElement) });
//		}
//	}
//}

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

		animationDictionary.insert({ animationIdString, animationSerialiserDictionary.at(animationTypeString)->Parse(*animationElement) });
	}
}


//const std::string& AnimationHolder::GetXMLString(std::string animationId) const
//{
//	NormaliseIDString(animationId);
//
//	auto it = stringDictionary.find(animationId);
//	if (it == stringDictionary.cend())
//		throw std::runtime_error("AnimationHolder: No animation exists under this id: " + animationId);
//
//	return it->second;
//}

const EntityAnimator::AnimationFunction& AnimationHolder::GetAnimation(std::string animationId) const
{
	NormaliseIDString(animationId);

	auto it = animationDictionary.find(animationId);
	if (it == animationDictionary.cend())
		throw std::runtime_error("AnimationHolder: No animation exists under this id: " + animationId);

	return it->second;
}