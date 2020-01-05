#pragma once

/// @file
/// @brief class mbe::FrameAnimationSerialiser

#include <memory>

#include <MBE/Serialisation/AnimationSerialiser.h>


namespace mbe
{

	// XML format
	// <Animation type="FrameAnimation" id="string" duration="int">
	//		<Frame>
	//			<Duration>float</Duration>
	//			<SubRect>
	//				<Top>int</Top>
	//				<Left>int</Left>
	//				<Width>int</Width>
	//				<Height>int</Height>
	//			</SubRect>
	//			<!-- optional -->
	//			<Origin x="float" y="float />
	//		</Frame>
	// ...
	// </Animation>
	class FrameAnimationSerialiser : public AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<FrameAnimationSerialiser> Ptr;
		typedef std::weak_ptr<FrameAnimationSerialiser> WPtr;
		typedef std::unique_ptr<FrameAnimationSerialiser> UPtr;

	public:
		FrameAnimationSerialiser() = default;
		~FrameAnimationSerialiser() = default;

	public:
		void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) override;

		void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) override;
	};


} // namespace mbe