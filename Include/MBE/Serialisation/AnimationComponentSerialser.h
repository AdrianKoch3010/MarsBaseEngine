#pragma once

/// @file
/// brief mbe::AnimationComponentSerialiser

#include <string>
#include <unordered_map>
#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Serialisation/AnimationSerialiser.h>


// only for testing
#include <MBE/AI/AIComponent.h>
#include <MBE/Serialisation/AITaskSerialser.h>

namespace mbe
{
	/// @brief Component serialiser for the mbe::AnimationComponent
	/// @details By default this class provides serialisers for the
	/// - FrameAnimation
	/// - BlinkingAnimation
	/// - RotationAnimation
	/// - PitchAnimation
	/// @n Custom animation serialisers can be added using the AddAnimationSerialiser() method and must inherit from mbe::AnimationSerialiser
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
	///				<Animation type="string" id="string" duration="int in milliseconds">
	///					// Animation data
	///				</Animation>
	///			</Animations>
	///		</Animator>
	/// </Component>
	/// @endcode
	class AnimationComponentSerialiser : public ComponentSerialser
	{
	private:
		typedef std::unordered_map<std::string, AnimationSerialiser::UPtr> AnimationSerialiserDictionary;
		typedef std::unordered_map<EntityAnimator::AnimationTypeID, std::string> AnimationTypeDictionary;

	public:
		typedef std::shared_ptr<AnimationComponentSerialiser> Ptr;
		typedef std::weak_ptr<AnimationComponentSerialiser> WPtr;
		typedef std::unique_ptr<AnimationComponentSerialiser> UPtr;

	public:
		/// @brief Constructor
		AnimationComponentSerialiser();

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
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		/// @brief Stores the entity's animation component to an XML element
		/// @details Stores all animators and their animations
		/// @param entity The entity whos animation component is stored. This entity must have an mbe::AnimationComponent
		/// @param document The XML document to which the animation component is stored
		/// @param componentData The XML element to which the animation component is stored
		/// @throws std::runtime_error if the entity doesn't have an mbe::AnimationComponent or when there was no animation serialiser registered for a given animation type
		/// @see AddAnimationSerialiser
		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

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
		AnimationSerialiserDictionary animationSerialiserDictionary;
		AnimationTypeDictionary animationTypeDictionary;
	};

#pragma region Template Implementation

	template<class TAnimationSerialiser, class TAnimation, typename ...TArguments>
	inline void AnimationComponentSerialiser::AddAnimationSerialiser(const std::string& animationType, TArguments ...arguments)
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

#pragma endregion


} // namespace mbe