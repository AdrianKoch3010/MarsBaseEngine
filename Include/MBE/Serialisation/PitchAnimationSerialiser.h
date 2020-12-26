#pragma once

/// @file
/// @brief class mbe::PitchAnimationSerialiser

#include <memory>

#include <MBE/Serialisation/AnimationSerialiser.h>


namespace mbe
{

	// XML format
	// <Animation type="PitchAnimation" id="string" duration="int">
	//		<MinimumPitch>float</MinimumPitch>
	// </Animation>
	class PitchAnimationSerialiser : public AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<PitchAnimationSerialiser> Ptr;
		typedef std::weak_ptr<PitchAnimationSerialiser> WPtr;
		typedef std::unique_ptr<PitchAnimationSerialiser> UPtr;

	public:
		PitchAnimationSerialiser() = default;
		~PitchAnimationSerialiser() = default;

	public:
		void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const override;

		void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const override;

		EntityAnimator::AnimationFunction Parse(const tinyxml2::XMLElement& animationData) const override;
	};

} // namespace mbe