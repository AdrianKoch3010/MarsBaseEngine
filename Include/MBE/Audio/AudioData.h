//#pragma once
//
///// @file
///// @brief Class mbe::AudioData
//
//#include <string>
//#include <memory>
//
//#include <SFML/System/Time.hpp>
//
//#include <MBE/Core/Utility.h>
//
//
//namespace mbe
//{
//	// Formula used to determine the volume based of minDistance, distance and attenuation
//	// Volume factor = MinDistance / (MinDistance + Attenuation * (max(Distance, MinDistance) - MinDistance))
//	class AudioData
//	{
//	public:
//		enum class Type : unsigned short int
//		{
//			Sound,
//			Music
//		};
//
//		enum class AudioStatus : unsigned short int
//		{
//			Playing,
//			Paused,
//			Stopped
//		};
//
//	public:
//		typedef std::shared_ptr<AudioData> Ptr;
//		typedef std::weak_ptr<AudioData> WPtr;
//		typedef std::unique_ptr<AudioData> UPtr;
//		typedef std::string ID;
//
//	public:
//		AudioData(ID soundBufferId, Type type, float attenuation, float minDistance, float volume, float pitch, sf::Time playingOffset, bool looped);
//
//		// Default copy constructor
//		AudioData(const AudioData & obj) = default;
//
//	public:
//		void SetAudioSourceID(ID soundBufferId);
//		inline const ID & GetAudioSourceID() const { return audioSourceId; }
//
//		// The position relative to the mbe::TransformComponent
//		Type audioType;
//		float attenuation;
//		float minDistance;
//		float volume;
//		float pitch;
//		sf::Time playingOffset;
//		bool looped;
//
//		// Static default object containing the default values
//		// Not const so can be changed by the user of the API
//		static AudioData Default;
//
//	private:
//		ID audioSourceId;
//	};
//
//} // namespace mbe
//
///// example:
///// auto audioData = mbe::AudioData::Default;
///// audioData.SetSoundSourceID("ID");
///// audioData.relativePosition = sf::Vector2f(11.1f, 22.2f);

#pragma once

/// @file
/// @brief Class mbe::AudioData

#include <cmath>
#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

namespace mbe
{
	// Static class containing the global information on Audio Data
	/// @brief Static class containing global audio information
	/// @details Stores the
	/// - listener position
	/// - total number of sf::Sound and sf::Music instances present at a time
	/// @n It also provides functionality for converting between 2D and 3D minimum distances
	class AudioData
	{
	public:
		enum class AudioStatus : unsigned short int
		{
			Playing,
			Paused,
			Stopped
		};

	public:
		AudioData() = default;
		~AudioData() = default;

	public:
		static void SetListenerPosition(const sf::Vector2f & position);

		inline static void SetListenerZPosition(float value) { listenerZ = value; }

		static sf::Vector2f GetListenerPosition();

		inline static float GetListenerZPosition() { return listenerZ; }

		// The number of current sounds is not guaranteed to be correct, the user has to take care to increment the 
		inline static unsigned short int GetCurrentNumberOfSounds() { return totalNumberOfSounds; }

		inline static void IncrementNumberOfSounds() { totalNumberOfSounds++; }

		inline static void DecrementNumberOfSounds() { totalNumberOfSounds--; }

		static float Convert2DTo3DMinDistance(float minDistance2d);

		static float Convert3DTo2DMinDistance(float minDistance3d);

		inline static unsigned short int GetMaxNumberOfSounds() { return maxNumberOfSounds; }

	private:
		static float listenerZ;

		static unsigned short int totalNumberOfSounds;
		static const unsigned short int maxNumberOfSounds;
	};

} // namespace mbe