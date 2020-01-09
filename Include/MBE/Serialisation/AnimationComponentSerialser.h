#pragma once

#include <string>
#include <unordered_map>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Serialisation/AnimationSerialiser.h>

namespace mbe
{
	// Only store mbe::EntityAnimators
	//
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
		// Add mbe animation serialisers
		// Add the mbe Animation serialisers
		AnimationComponentSerialiser();
		~AnimationComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

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
			throw std::runtime_error("EntitySerialiser: An animation serialser already exists for this animation type (" + animationType + ")");

		// Remember the typeId for this animation type for the serialiser store function
		animationTypeDictionary.insert({ detail::GetAnimationTypeID<TAnimation>(), animationType });

		// Make a new component serialser
		auto animationSerialiserPtr = std::make_unique<TAnimationSerialiser>(std::forward<TArguments>(arguments)...);

		// Add the component serialiser to the dictionary
		animationSerialiserDictionary.insert({ animationType, std::move(animationSerialiserPtr) });
	}

#pragma endregion

} // namespace mbe