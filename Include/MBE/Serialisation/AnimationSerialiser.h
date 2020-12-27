#pragma once

/// @file
/// @brief class mbe::AnimationSerialiser

#include <memory>
#include <string>

// TinyXML2
#include <MBE/Core/TinyXML.h>

#include <MBE/Core/Entity.h>
#include <MBE/Animation/EntityAnimator.h>

#include <MBE/Core/StaticBlock.h>
#include <MBE/Core/Singleton.h>

namespace mbe { class AnimationSerialiserRegistry; }

/// @brief A macro for adding an animation serialiser to the global registry
/// @details This is meant to be put in the .cpp file of the animation serialiser. The animation serialiser will be registered
/// under the string name of the animation
/// @attention An animation serialiser can only be registred once, hence, this macro must only be compiled once
//#define MBE_REGISTER_ANIMATION_SERIALISER(_AnimationSerialiser, _Animation) static_block{ mbe::AnimationSerialiserRegistry::Instance().AddAnimationSerialiser<_AnimationSerialiser, _Animation>(MBE_NAME_OF(_Animation)); }


namespace mbe
{
	/// @brief Interface for loading and storing animation data from and to an XML file format
	class AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<AnimationSerialiser> Ptr;
		typedef std::weak_ptr<AnimationSerialiser> WPtr;
		typedef std::unique_ptr<AnimationSerialiser> UPtr;

	public:
		/// @brief Default constructor
		AnimationSerialiser() = default;

		/// @brief Default destructor
		~AnimationSerialiser() = default;

	public:
		/// @brief Interface for loading and adding an animation to an entity animator
		/// @param entityAnimator The entity animator to which the animation is added
		/// @param animationData The XML element which is parsed and used to determine the value of the animation
		/// @param animationId The id of the animation
		/// @param duration The duration of the animation
		virtual void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const = 0;

		/// @brief Interface for serialising an animation into an XML element
		/// @param entityAnimator The animator whos animation is stored.
		/// @param animationId The id of the animation to be stored
		/// @param document The XML document that the animation is stored to
		/// @param animationData The XML element to which the animation is stored to
		virtual void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const = 0;
		
		/// @brief Interface for parsing a single animation template
		/// @param animationData The XML element which is parsed and then converted to the respective animation function
		/// @return The animation function object
		virtual EntityAnimator::AnimationFunction Parse(const tinyxml2::XMLElement& animationData) const = 0;

	};

} // namespace mbe