#pragma once

/// @file
/// @brief Class mbe::AudioData

#include <string>
#include <memory>

#include <SFML/System/Time.hpp>

#include <MBE/Core/Utility.h>


namespace mbe
{
	// Formula used to determine the volume based of minDistance, distance and attenuation
	// Volume factor = MinDistance / (MinDistance + Attenuation * (max(Distance, MinDistance) - MinDistance))
	class AudioData
	{
	public:
		enum class Type : unsigned short int
		{
			Sound,
			Music
		};

		enum class AudioStatus : unsigned short int
		{
			Playing,
			Paused,
			Stopped
		};

	public:
		typedef std::shared_ptr<AudioData> Ptr;
		typedef std::string ID;

	public:
		AudioData(ID soundBufferId, Type type, float attenuation, float minDistance, float volume, float pitch, sf::Time playingOffset, bool looped);

		// Default copy constructor
		AudioData(const AudioData & obj) = default;

	public:
		void SetAudioSourceID(ID soundBufferId);
		inline const ID & GetAudioSourceID() const { return audioSourceId; }

		// The position relative to the mbe::TransformComponent
		Type audioType;
		float attenuation;
		float minDistance;
		float volume;
		float pitch;
		sf::Time playingOffset;
		bool looped;

		// Static default object containing the default values
		// Not const so can be changed by the user of the API
		static AudioData Default;

	private:
		ID audioSourceId;
	};

} // namespace mbe

/// example:
/// auto audioData = mbe::AudioData::Default;
/// audioData.SetSoundSourceID("ID");
/// audioData.relativePosition = sf::Vector2f(11.1f, 22.2f);