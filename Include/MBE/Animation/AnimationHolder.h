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
		typedef std::unordered_map<std::string, std::string> StringDictionary;
		typedef std::unordered_map<std::string, EntityAnimator::AnimationFunction> AnimationDictionary;
		typedef std::unordered_map<std::string, AnimationSerialiser::UPtr> AnimationSerialiserDictionary;
		typedef std::unordered_map<EntityAnimator::AnimationTypeID, std::string> AnimationTypeDictionary;

	public:
		typedef std::shared_ptr<AnimationHolder> Ptr;
		typedef std::weak_ptr<AnimationHolder> WPtr;
		typedef std::unique_ptr<AnimationHolder> UPtr;

	public:
		/// @brief Constructor
		AnimationHolder();

		/// @brief Default destructor
		~AnimationHolder() = default;

	public:
		// The fileName is the name of the file that contains the file names of the files containing the animation XML

		/// @brief 
		/// @param filePath 
		/// @param fileName 
		//void Load(const std::string& filePath, std::string fileName = "Animations.txt");

		void Load(std::string file);

		// This id is different to the one used within an animator

		/// @brief Adds an animation template to the list of managed animations
		/// @tparam The type of the animation being added
		/// @param animationId The global id of the animation template
		/// @param xmlString The animation template in xml format
		template<class TAnimation>
		void AddAnimation(std::string animationId, const std::string& xmlString);

		/// @brief Returns the xml string of the managed animation template
		/// @param animationId The global id of the animation template
		/// @return The corresponding XML string
		//const std::string& GetXMLString(std::string animationId) const;

		/// @brief Returns a reference to the managed animation template
		/// @param animationId The global id of the animation template
		/// @return A reference to the animation function
		const EntityAnimator::AnimationFunction& GetAnimation(std::string animationId) const;

		/// @brief Same as GetAnimation()
		/// @see GetAnimation()
		/// @param animationID The global id of the animation template
		/// @return A reference to the animation function
		inline const EntityAnimator::AnimationFunction& operator[](std::string& animationID) const { return GetAnimation(animationID); }

		/// @brief Allows for registering custom animation serialisers
		/// @details When loading loading from XML, the type name of the animation is looked up. This must be equivalent to the animationType
		/// argument passed to this method. When storing, the type of animation is looked up. This must be equivalent to the TAnimation template parameter.
		/// @tparam TAnimationSerialiser The type of the custom animation serialiser
		/// @tparam TAnimation The type of the animation for which this serialiser is added
		/// @tparam TArguments The type of any arguments that may be passed to the custion animation serialiser's constructor
		/// @param animationType The type name used when storing this animation to XML
		/// @param arguments Any arguments that may be passed to the custion animation serialiser's constructor
		/// @throws std::runtime_error if an animation serialiser already exists for the animation type (both the type string and TAnimation type)
		/// @see LoadComponent, StoreComponent
		template<class TAnimationSerialiser, class TAnimation, typename... TArguments>
		void AddAnimationSerialiser(const std::string& animationType, TArguments... arguments);

	private:
		//StringDictionary stringDictionary;
		AnimationDictionary animationDictionary;
		AnimationSerialiserDictionary animationSerialiserDictionary;
		AnimationTypeDictionary animationTypeDictionary;
	};

#pragma region Template Implementation

	template<class TAnimationSerialiser, class TAnimation, typename ...TArguments>
	inline void AnimationHolder::AddAnimationSerialiser(const std::string& animationType, TArguments ...arguments)
	{
		// make sure that TAnimationSerialiser inherits from mbe::AnimationSerialiser
		static_assert(std::is_base_of<AnimationSerialiser, TAnimationSerialiser>::value, "The animation serialiser must inherit from mbe::AnimationSerialiser");

		// Throw if a component serialiser for this type already exists
		if (animationSerialiserDictionary.find(animationType) != animationSerialiserDictionary.end())
			throw std::runtime_error("AnimationComponentSerialiser: An animation serialser already exists for this animation type (" + animationType + ")");

		if (animationTypeDictionary.find(detail::GetAnimationTypeID<TAnimation>()) != animationTypeDictionary.cend())
			throw std::runtime_error("AnimationComponentSerialiser: An animation serialser already exists for this animation type(" + std::to_string(detail::GetAnimationTypeID<TAnimation>()) + ")");

		// Remember the typeId for this animation type for the serialiser store function
		animationTypeDictionary.insert({ detail::GetAnimationTypeID<TAnimation>(), animationType });

		// Make a new component serialser
		auto animationSerialiserPtr = std::make_unique<TAnimationSerialiser>(std::forward<TArguments>(arguments)...);

		// Add the component serialiser to the dictionary
		animationSerialiserDictionary.insert({ animationType, std::move(animationSerialiserPtr) });
	}

	template<class TAnimation>
	inline void AnimationHolder::AddAnimation(std::string animationId, const std::string& xmlString)
	{
		// Store the xml string
		NormaliseIDString(globalId);

		auto it = animationDictionary.find(globalId);
		if (it == animationDictionary.cend())
			throw std::runtime_error("AnimationHolder: An animation has already been added for this id: " + globalId);

		//stringDictionary.insert({ globalId, xmlString });

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
		auto typeItr = animationTypeDictionary.find(detail::GetAnimationTypeID<TAnimation>());
		if (typeItr == animationTypeDictionary.cend())
			throw std::runtime_error("AnimationHolder: No animation serialiser has been registered for this animation type: " + std::to_string(detail::GetAnimationTypeID<TAnimation>()));

		auto animationTypeString = *typeItr;

		animationDictionary.insert({ animationId, animationSerialiserDictionary.at(animationTypeString)->Parse(*rootNode) });
	}

#pragma endregion

} // namespace mbe