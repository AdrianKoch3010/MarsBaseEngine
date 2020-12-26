#pragma once

/// @file
/// @brief class mbe::BlinkingAnimationSerialiser

#include <memory>

#include <MBE/Serialisation/AnimationSerialiser.h>


namespace mbe
{

	// XML format
	// <Animation type="BlinkingAnimation" id="string" duration="int">
	//		<MinimumBrightness>float</MinimumBrightness>
	// </Animation>
	class BlinkingAnimationSerialiser : public AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<BlinkingAnimationSerialiser> Ptr;
		typedef std::weak_ptr<BlinkingAnimationSerialiser> WPtr;
		typedef std::unique_ptr<BlinkingAnimationSerialiser> UPtr;

	public:
		BlinkingAnimationSerialiser() = default;
		~BlinkingAnimationSerialiser() = default;

	public:
		void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const override;

		void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const override;
	
		EntityAnimator::AnimationFunction Parse(const tinyxml2::XMLElement& animationData) const override;
	};

} // namespace mbe