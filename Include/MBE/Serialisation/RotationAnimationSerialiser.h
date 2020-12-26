#pragma once

/// @file
/// @brief class mbe::RotationAnimationSerialiser

#include <memory>

#include <MBE/Serialisation/AnimationSerialiser.h>


namespace mbe
{

	// XML format
	// <Animation type="RotationAnimation" id="string" duration="int">
	//		<RelativeMaximumRotation>float</RelativeMaximumRotation>
	// </Animation>
	class RotationAnimationSerialiser : public AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<RotationAnimationSerialiser> Ptr;
		typedef std::weak_ptr<RotationAnimationSerialiser> WPtr;
		typedef std::unique_ptr<RotationAnimationSerialiser> UPtr;

	public:
		RotationAnimationSerialiser() = default;
		~RotationAnimationSerialiser() = default;

	public:
		void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const override;

		void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const override;
	
		EntityAnimator::AnimationFunction Parse(const tinyxml2::XMLElement& animationData) const override;
	};

} // namespace mbe