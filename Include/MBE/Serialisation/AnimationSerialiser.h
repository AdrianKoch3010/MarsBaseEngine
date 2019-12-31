#pragma once

/// @file
/// @brief class mbe::AnimationSerialiser

#include <memory>

// TinyXML2
#include <MBE/Core/TinyXML.h>

#include <MBE/Core/Entity.h>
#include <MBE/Animation/EntityAnimator.h>

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
		virtual void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) = 0;

		/// @brief Interface for serialising an animation into an XML element
		/// @param entityAnimator The animator whos animation is stored.
		/// @param document The XML document that the animation is stored to
		/// @param animationData The XML element to which the animation is stored to
		virtual void Store(const EntityAnimator& entityAnimator, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) = 0;
	};


} // namespace mbe