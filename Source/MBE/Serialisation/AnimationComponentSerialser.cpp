#include <string>
#include <exception>
#include <SFML/System/Time.hpp>

#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Animation/AnimationHolder.h>

#include <MBE/Serialisation/SerialiserRegistry.h>
#include <MBE/Serialisation/AnimationComponentSerialser.h>

using namespace mbe;

void AnimationComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	// Add the animation component
	auto& animationComponent = entity.AddComponent<AnimationComponent>();

	// Load the list of entities
	for (auto animatorElement = componentData.FirstChildElement("Animator"); animatorElement != nullptr; animatorElement = animatorElement->NextSiblingElement("Animator"))
	{
		// Get the animator id
		auto animatorId = animatorElement->Attribute("id");
		if (animatorId == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator id");
		std::string animatorIdString{ animatorId };

		// Add the animator
		auto& animator = animationComponent.CreateAnimator(animatorIdString);

		// Get the default values from the animator
		bool loop = animator.IsLooping();
		bool paused = animator.IsPaused();
		float progress = animator.GetProgress();
		bool isPlayingAnimation = false;

		// Get Loop
		const auto loopElement = animatorElement->FirstChildElement("Loop");
		if (loopElement != nullptr)
			if (loopElement->QueryBoolText(&loop) != XML_SUCCESS)
				throw std::runtime_error("Load animation component: Failed to parse animator loop text");

		// Get Paused
		const auto pausedElement = animatorElement->FirstChildElement("Paused");
		if (pausedElement != nullptr)
			if (pausedElement->QueryBoolText(&paused) != XML_SUCCESS)
				throw std::runtime_error("Load animation component: Failed to parse animator paused text");

		// Get Progress
		const auto progressElement = animatorElement->FirstChildElement("Progress");
		if (progressElement != nullptr)
			if (progressElement->QueryFloatText(&progress) != XML_SUCCESS)
				throw std::runtime_error("Load animation component: Failed to parse animator progress text");

		// Get the currently playing animation
		// If none exists, the animator is stopped
		std::string currentlyPlayingAnimationString;
		const auto currentAnimationElement = animatorElement->FirstChildElement("CurrentlyPlayingAnimation");
		if (currentAnimationElement == nullptr)
			isPlayingAnimation = false;
		else
		{
			isPlayingAnimation = true;
			auto currentlyPlayingAnimation = currentAnimationElement->GetText();
			if (currentlyPlayingAnimation == nullptr)
				throw std::runtime_error("Load animation component: Failed to parse currently playing animation");
			currentlyPlayingAnimationString = currentlyPlayingAnimation;
		}

		// Load recreate the amimations
		auto animationsElement = animatorElement->FirstChildElement("Animations");
		if (animationsElement == nullptr)
			throw std::runtime_error("Load animation component: Failed to parse animator animations element");

		for (auto animationElement = animationsElement->FirstChildElement("Animation"); animationElement != nullptr; animationElement = animationElement->NextSiblingElement("Animation"))
		{
			// Get the animation id
			auto animationId = animationElement->Attribute("id");
			if (animationId == nullptr)
				throw std::runtime_error("Load animation component: Failed to parse animation id");
			std::string animationIdString{ animationId };

			// Get the animation duration
			int duration;
			if (animationElement->QueryIntAttribute("duration", &duration) != XML_SUCCESS)
				throw std::runtime_error("Load animation component: Failed to parse animation time");

			// Check whether a global animation id is used
			bool usingGlobalId = false;
			std::string globalAnimationIdString = "";
			auto globalAnimationId = animationElement->Attribute("globalId");
			if (globalAnimationId != nullptr)
			{
				usingGlobalId = true;
				globalAnimationIdString = std::string(globalAnimationId);
			}

			// When using a global id for the animation, the corresponding XMLElement must be looked up
			if (usingGlobalId)
			{
				animator.AddLocalAnimation(animationIdString, globalAnimationIdString, sf::milliseconds(duration));
			}
			else
			{
				// Get the animation type
				auto animationType = animationElement->Attribute("type");
				if (animationType == nullptr)
					throw std::runtime_error("Load animation component: Failed to parse animation type");
				std::string animationTypeString{ animationType };

				// Call the appropreate animation serialiser for that animation type
				AnimationSerialiserRegistry::Instance()[animationTypeString].Load(animator, *animationElement, animationIdString, sf::milliseconds(duration));
			}
		}

		// Set the animator state
		if (isPlayingAnimation)
			animator.PlayAnimation(currentlyPlayingAnimationString, loop);
		animator.SetPaused(paused);
		animator.SetProgress(progress);
		// Is this necessary
		animator.SetLooping(loop);
	}
}

void AnimationComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have an animation component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<AnimationComponent>() == false)
		throw std::runtime_error("AnimationComponentSerialiser: Store animation component: The entity must have an mbe::AnimationComponent");
	const auto& animationComponent = entity.GetComponent<AnimationComponent>();

	// Store the animators
	for (const auto& pair : animationComponent.GetAnimatorDictionary())
	{
		assert(pair.second != nullptr && "AnimationComponentSerialiser: The animator must exist");
		const auto& animator = *pair.second;

		// Store animator
		auto animatorElement = document.NewElement("Animator");
		animatorElement->SetAttribute("id", pair.first.c_str());
		componentData.InsertEndChild(animatorElement);

		// Store progress
		auto progressElement = document.NewElement("Progress");
		progressElement->SetText(animator.GetProgress());
		animatorElement->InsertEndChild(progressElement);

		// Store loop
		auto loopElement = document.NewElement("Loop");
		loopElement->SetText(animator.IsLooping());
		animatorElement->InsertEndChild(loopElement);

		// Store paused
		auto pausedElement = document.NewElement("Paused");
		pausedElement->SetText(animator.IsPaused());
		animatorElement->InsertEndChild(pausedElement);

		// Store currently playing animation (if there is one)
		if (animator.IsPlayingAnimation())
		{
			auto currentlyPlayingAnimationElement = document.NewElement("CurrentlyPlayingAnimation");
			currentlyPlayingAnimationElement->SetText(animator.GetPlayingAnimation().c_str());
			animatorElement->InsertEndChild(currentlyPlayingAnimationElement);
		}

		// Store the animations
		auto animationsElement = document.NewElement("Animations");
		animatorElement->InsertEndChild(animationsElement);

		for (const auto& animationPair : animator.GetAnimationDictionary())
		{
			const auto& animationId = animationPair.first;
			const auto& animationDuration = animationPair.second.second;

			
			// Store animation type and id
			auto animationElement = document.NewElement("Animation");
			animationElement->SetAttribute("id", animationId.c_str());

			// Store animation time
			animationElement->SetAttribute("duration", animationDuration.asMilliseconds());

			// Check if a global id exists for this animator
			if (animator.HasGlobalAnimationID(animationId))
			{
				// Only store the global animation id
				animationElement->SetAttribute("globalId", animator.GetGlobalAnimationID(animationId).c_str());
			}
			else
			{
				const auto& animationTypeString = AnimationSerialiserRegistry::Instance().GetObjectName(animator.GetLocalAnimationTypeID(animationId));

				animationElement->SetAttribute("type", animationTypeString.c_str());
				// Call the corresponding animation serialiser
				AnimationSerialiserRegistry::Instance()[animationTypeString].Store(animator, animationId, document, *animationElement);
			}

			animationsElement->InsertEndChild(animationElement);
		}
	}
}