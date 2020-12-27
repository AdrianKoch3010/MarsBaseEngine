#pragma once

#include <string>
#include <unordered_map>

#include <MBE/Core/Singleton.h>
#include <MBE/Serialisation/AnimationSerialiser.h>
#include <MBE/Animation/EntityAnimator.h>

namespace mbe
{

	/// @brief The global registry for animation serialisers
	/// @details Since the animation serialiser registry is a singleton it can be accessed from anywhere using AnimationSerialiserRegistry::Instance().
	/// @note In order to register an mbe::AnimationSerialiser the MBE_REGISTER_ANIMATION_SERIALISER macro should be used
	class AnimationSerialiserRegistry : public detail::Singleton<AnimationSerialiserRegistry>
	{
	private:
		friend class detail::Singleton<AnimationSerialiserRegistry>;

	private:
		typedef std::unordered_map<std::string, AnimationSerialiser::UPtr> AnimationSerialiserDictionary;
		typedef std::unordered_map<EntityAnimator::AnimationTypeID, std::string> AnimationTypeDictionary;

	public:
		/// @brief Constructor
		/// @details Registers the known animation serialisers
		AnimationSerialiserRegistry();

	public:
		/// @brief Allows for registering custom animation serialisers
		/// @details When loading loading from XML, the type name of the animation is looked up. This must be equivalent to the animationType
		/// argument passed to this method. When storing, the type of animation is looked up. This must be equivalent to the TAnimation template parameter.
		/// @tparam TAnimationSerialiser The type of the custom animation serialiser
		/// @tparam TAnimation The type of the animation for which this serialiser is added
		/// @tparam TArguments The type of any arguments that may be passed to the custion animation serialiser's constructor
		/// @param animationType The type name used when storing this animation to XML
		/// @param arguments Any arguments that may be passed to the custion animation serialiser's constructor
		/// @throws std::runtime_error if an animation serialiser already exists for the animation type (both the type string and TAnimation type)
		/// @note In order to register an mbe::AnimationSerialiser the MBE_REGISTER_ANIMATION_SERIALISER macro should be used
		/// @see LoadComponent, StoreComponent
		template<class TAnimationSerialiser, class TAnimation, typename... TArguments>
		void AddAnimationSerialiser(const std::string& animationType, TArguments... arguments);

		/// @brief Exposes the animation serialiser dictionary
		/// @return A dictionary mapping the name of the animation serialiser to the stored instance
		inline const AnimationSerialiserDictionary& GetAnimationSerialiserDictionary() { return animationSerialiserDictionary; }

		/// @brief Exposes the animation type dictionary
		/// @return A dictionary mapping the animation type id to the name of the animation serialiser
		inline const AnimationTypeDictionary& GetAnimationTypeDictionary() { return animationTypeDictionary; }

	private:
		AnimationSerialiserDictionary animationSerialiserDictionary;
		AnimationTypeDictionary animationTypeDictionary;
	};

	//namespace AnimationSerialiserRegistrations
	//{
	//	template<class TAnimationSerialiser, class TAnimation>
	//	class AnimationSerialiserRegistration
	//	{
	//	public:
	//		AnimationSerialiserRegistration(std::string animationType) {
	//			std::cerr << "Registered animation serilaiser for: " << animationType << std::endl;
	//			AnimationSerialiserRegistry::Instance().AddAnimationSerialiser<TAnimationSerialiser, TAnimation>(animationType);
	//		}
	//	};
	//}
	

#pragma region Template Implementation

	template<class TAnimationSerialiser, class TAnimation, typename ...TArguments>
	inline void AnimationSerialiserRegistry::AddAnimationSerialiser(const std::string& animationType, TArguments ...arguments)
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