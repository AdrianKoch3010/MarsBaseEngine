#pragma once

#include <string>
#include <unordered_map>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Animation/AnimationComponent.h>
#include <MBE/Serialisation/AnimationSerialiser.h>

namespace mbe
{
	/*class AnimatorSerialiser
	{
		struct Data
		{
			Entity::HandleID animatedObjectId;
			bool active;

			AnimationMap animationDictionary;
			AnimationMapIterator currentlyPlayingAnimation;
			float progress;
			bool loop;
			bool paused;
		};

		template<class TAnimationSerialiser, class TAnimation, typename... TArguments>
		void RegisterAnimationSerialiser(const std::string& animation, TArguments&& ...arguments);
	};*/

	// Only store mbe::Animator<mbe::Entity>
	//
	/// @code
	/// <Component type="AnimationComponent">
	///		<Animator id"string">
	///			<Active>bool</Active>
	///			<Progress>float>/Progress>
	///			<Loop>bool</Loop>
	///			<Paused>bool</Paused>
	///			<Animations>
	///				<Animation type="string" id="string">
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

	public:
		// Add mbe animation serialisers
		AnimationComponentSerialiser() = default;
		~AnimationComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

		template<class TComponentSerialiser, class TComponent, typename... TArguments>
		void AddAnimationSerialiser(const std::string& componentType, TArguments... arguments);

	private:
		AnimationSerialiserDictionary animationSerialiserDictionary;

	};

#pragma region Template Implementation

	template<class TComponentSerialiser, class TComponent, typename ...TArguments>
	inline void AnimationComponentSerialiser::AddAnimationSerialiser(const std::string& componentType, TArguments ...arguments)
	{

	}

#pragma endregion

} // namespace mbe