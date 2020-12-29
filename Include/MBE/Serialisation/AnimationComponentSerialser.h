#pragma once

/// @file
/// brief mbe::AnimationComponentSerialiser

#include <string>
#include <unordered_map>
#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Animation/AnimationHolder.h>


namespace mbe
{
	/// @brief Component serialiser for the mbe::AnimationComponent
	/// @n Animations can also be stored in an animation holder in which case they are referred to purly by their globalId. In this case,
	/// the animation serialisers are not needed.
	/// @n Custom animation serialisers can be registered with the mbe::AnimationSerialiserRegistry and must inherit from mbe::AnimationSerialiser
	/// @n@n **XML format**
	/// @code
	/// <Component type="AnimationComponent">
	///		<Animator id"string">
	///			<Progress>float>/Progress>
	///			<Loop>bool</Loop>
	///			<Paused>bool</Paused>
	///			<!-- Optional -->
	///			<CurrentlyPlayingAnimation>string</CurrentlyPlayingAnimation>
	///			<Animations>
	///				<!-- Either -->
	///				<Animation type="string" id="string" duration="int in milliseconds">
	///					// Animation data
	///				</Animation>
	///				<!-- Or -->
	///				<Animation id="string" duration="int in milliseconds" globalId="string" />
	///			</Animations>
	///		</Animator>
	/// </Component>
	/// @endcode
	class AnimationComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<AnimationComponentSerialiser> Ptr;
		typedef std::weak_ptr<AnimationComponentSerialiser> WPtr;
		typedef std::unique_ptr<AnimationComponentSerialiser> UPtr;

	public:
		/// @brief Defaut Constructor
		AnimationComponentSerialiser() = default;

		/// @brief Default destructor
		~AnimationComponentSerialiser() = default;

	public:
		/// @brief Loads the animation component data and adds a new mbe::AnimationComponent to the entity
		/// @details The XML element stores all animators and their animations. This method will create the amimators
		/// and add the animations to them. Custom animation serialisers can be added using the AddAnimationSerialiser() method.
		/// @param entity The entity to which the animation component is added
		/// @param componentData The XML element being parsed
		/// @throws std::runtime_error if the parsing fails or when there was no animation serialiser registered for a given animation type
		/// @see AddAnimationSerialiser
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		/// @brief Stores the entity's animation component to an XML element
		/// @details Stores all animators and their animations
		/// @param entity The entity whos animation component is stored. This entity must have an mbe::AnimationComponent
		/// @param document The XML document to which the animation component is stored
		/// @param componentData The XML element to which the animation component is stored
		/// @throws std::runtime_error if the entity doesn't have an mbe::AnimationComponent or when there was no animation serialiser registered for a given animation type
		/// @see AddAnimationSerialiser
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;
	};

} // namespace mbe