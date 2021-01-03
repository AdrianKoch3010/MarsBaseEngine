#pragma once

/// @file
/// @brief class mbe::FrameAnimationSerialiser

#include <memory>

#include <MBE/Serialisation/AnimationSerialiser.h>


namespace mbe
{
	class FrameAnimation;

	/// @brief Animation serialiser for the mbe::FrameAnimation
	/// @details There are two ways of adding frames; The frames can be added individually using the 'Frame' node with the duration denoting
	/// the relative duration compared to the other frames i.e the time that this frame will be shown will
	/// be proportional to (duration / sum(durations). Alternatively, a group of similar frames can be added using the 'Frames' node.
	/// The frames added in this mannor will have equal size and duration, going from left to right starting at the first position.
	/// The two ways of adding frames cannot be mixed.
	/// @n@n **XML format**
	/// @code
	/// <Animation type="FrameAnimation" id="string" duration="int">
	///		<Frame>
	///			<Duration>float</Duration>
	///			<SubRect>
	///				<Top>int</Top>
	///				<Left>int</Left>
	///				<Width>int</Width>
	///				<Height>int</Height>
	///			</SubRect>
	///			<!-- optional -->
	///			<Origin x="float" y="float />
	///		</Frame>
	///		...
	///		or (cannot be mixed)
	///		<Frames>
	///			<Size x="unsigned int" y="unsigned int" />
	///			<First x = "unsigned int" y = "unsigned int" />
	///			<NoOfFrames>unsigned int</NoOfFrames>
	///		</Frames>
	/// </Animation>
	/// @endcode
	class FrameAnimationSerialiser : public AnimationSerialiser
	{
	public:
		typedef std::shared_ptr<FrameAnimationSerialiser> Ptr;
		typedef std::weak_ptr<FrameAnimationSerialiser> WPtr;
		typedef std::unique_ptr<FrameAnimationSerialiser> UPtr;

	public:
		/// @brief Default constructor
		FrameAnimationSerialiser() = default;

		/// @brief Default destructor
		~FrameAnimationSerialiser() = default;

	public:
		/// @brief Loads and adds an animation to an entity animator
		/// @param entityAnimator The entity animator to which the animation is added
		/// @param animationData The XML element which is parsed and used to determine the value of the animation
		/// @param animationId The id of the animation
		/// @param duration The duration of the animation
		void Load(EntityAnimator& entityAnimator, const tinyxml2::XMLElement& animationData, const std::string& animationId, sf::Time duration) const override;

		/// @brief Serialises an animation into an XML element
		/// @param entityAnimator The animator whos animation is stored.
		/// @param animationId The id of the animation to be stored
		/// @param document The XML document that the animation is stored to
		/// @param animationData The XML element to which the animation is stored to
		void Store(const EntityAnimator& entityAnimator, const std::string& animationId, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& animationData) const override;
		
		/// @brief Parses a single animation template
		/// @param animationData The XML element which is parsed and then converted to the respective animation function
		/// @return The animation function object
		EntityAnimator::AnimationFunction Parse(const tinyxml2::XMLElement& animationData) const override;
	};

} // namespace mbe