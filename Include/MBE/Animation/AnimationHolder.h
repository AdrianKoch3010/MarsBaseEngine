#pragma once

/// @file
/// @brief Class mbe::AnimationHolder

#include <string>
#include <unordered_map>

#include <MBE/Core/TinyXML.h>

#include <MBE/Animation/EntityAnimator.h>
#include <MBE/Serialisation/AnimationSerialiser.h>

namespace mbe
{
	/// @brief Allows for loading and managing animation templates
	class AnimationHolder
	{
	private:
		typedef std::unordered_map<std::string, EntityAnimator::AnimationFunction> AnimationDictionary;

	public:
		typedef std::shared_ptr<AnimationHolder> Ptr;
		typedef std::weak_ptr<AnimationHolder> WPtr;
		typedef std::unique_ptr<AnimationHolder> UPtr;

	public:
		/// @brief Default Constructor
		AnimationHolder() = default;

		/// @brief Default destructor
		~AnimationHolder() = default;

	public:

		void Load(std::string file);

		/// @brief Adds an animation template to the list of managed animations
		/// @tparam The type of the animation being added
		/// @param animationId The global id of the animation template
		/// @param xmlString The animation template in xml format
		template<class TComponent>
		void AddAnimation(std::string animationId, const std::string& xmlString);

		/// @brief Returns a reference to the managed animation template
		/// @param animationId The global id of the animation template
		/// @return A reference to the animation function
		const EntityAnimator::AnimationFunction& GetAnimation(std::string animationId) const;

		/// @brief Same as GetAnimation()
		/// @see GetAnimation()
		/// @param animationID The global id of the animation template
		/// @return A reference to the animation function
		inline const EntityAnimator::AnimationFunction& operator[](std::string& animationID) const { return GetAnimation(animationID); }

	private:
		AnimationDictionary animationDictionary;
	};

#pragma region Template Implementation

	template<class TComponent>
	inline void AnimationHolder::AddAnimation(std::string animationId, const std::string& xmlString)
	{
		NormaliseIDString(globalId);

		auto it = animationDictionary.find(globalId);
		if (it == animationDictionary.cend())
			throw std::runtime_error("AnimationHolder: An animation has already been added for this id: " + globalId);

		// Parse the animation

		// Convert to XML element
		XMLDocument document;
		XMLError error = document.Parse(xmlString.c_str());

		if (error != XML_SUCCESS)
			throw std::runtime_error("AnimationHolder: Failed to parse animation");

		// The root node is the animation to be parsed
		const auto rootNode = document.FirstChildElement();
		if (rootNode == nullptr)
			throw std::runtime_error("AnimationHolder: The root node could not be found");

		// Get the animation type string
		auto typeItr = animationTypeDictionary.find(detail::GetAnimationTypeID<TComponent>());
		if (typeItr == animationTypeDictionary.cend())
			throw std::runtime_error("AnimationHolder: No animation serialiser has been registered for this animation type: " + std::to_string(detail::GetAnimationTypeID<TComponent>()));

		auto animationTypeString = *typeItr;

		animationDictionary.insert({ animationId, animationSerialiserDictionary.at(animationTypeString)->Parse(*rootNode) });
	}

#pragma endregion

} // namespace mbe