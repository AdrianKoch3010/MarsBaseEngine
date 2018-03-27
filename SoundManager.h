#pragma once

/// @file
/// @brief Class mbe::SoundManager

#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <functional>
#include <algorithm>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "AssetHolder.h"
#include "AudioData.h"
#include "FilePathDictionary.h"


// Sound coordinate system, point of view of a player in front of the screen:
// X = left; Y = up; Z = back (out of the screen)

namespace mbe
{
	// Manages sf::Sound and sf::Music instances and provides access to them via an id
	// Abstracts sf::Sound and sf::Music to a general audio type for which the audio data can be set and fetched
	class SoundManager : private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<SoundManager> Ptr;
		typedef std::weak_ptr<SoundManager> WPtr;
		typedef std::unique_ptr<SoundManager> UPtr;

		typedef FilePathDictionary<AudioData::ID> FilePathDictionary;

		// Maybe make the id type a template parameter
		typedef unsigned long long int ID;

	public:
		explicit SoundManager(const SoundBufferHolder<AudioData::ID> & soundHolder, const FilePathDictionary & musicDictionary);

		// Stops all playing sounds and music
		~SoundManager();

	public:
		// Play a non spatialised sound with default values
		ID PlaySound(const AudioData::ID & audioSourceId, AudioData::Type audioType = AudioData::Type::Sound);

		// Play a non spatialised sound with specialised values
		ID PlaySound(const AudioData & audioData);

		// Play a spatialised sound with default values
		ID PlaySound(const AudioData::ID & audioSourceId, const sf::Vector2f & position, AudioData::Type audioType = AudioData::Type::Sound);

		// Play a spatialised sound and pass in a sound data object determining how the sound is played
		ID PlaySound(const AudioData & audioData, const sf::Vector2f & position);

		// Throws if audioId could not be found
		// Independent of audio type
		// Has no effect when the sound / music is stopped
		void SetPaused(ID audioId, bool paused = true);

		// Throws if audioId could not be found
		// Independent of audio type
		void StopSound(ID audioId);

		// Throws if audioId could not be found
		// Independent of audio type
		// Only updates attenuation, volume, pitch and loop
		// For non-spacialised audio
		void UpdateSound(ID audioId, const AudioData & audioData);

		// throws if audioId could not be found
		// Independent of audio type
		// Only updates position, attenuation, volume, pitch and loop
		// For spacialised audio
		void UpdateSound(ID audioId, const AudioData & audioData, const sf::Vector2f & position);

		// Throws if audioId could not be found
		// Independent of audio type
		// Note that stopped sound don't exist hence throws since the audio cannot be found
		AudioData::AudioStatus GetAudioStatus(ID audioId);

		// Throws if audioId could not be found
		// No return by reference since the audio data is constructed within the function
		// Note that the playing offset value represents the current playing offset (not the initial value)
		AudioData GetAudioData(ID audioId);

		// Throws if audioId could not be found
		// Independent of audio type
		sf::Vector2f GetAudioPosition(ID audioId);

		// This function should be called before pausing, stopping, updating or getting any data from an audio id
		// in order to make sure that it actually exists. This makes sure that the other methods won't throw
		/// find a better function name
		bool DoesExists(ID audioId);

		void RemoveStoppedSounds();

		static void SetListenerPosition(const sf::Vector2f & position);

		inline static void SetListenerZPosition(float value) { listenerZ = value; }

		static sf::Vector2f GetListenerPosition();

		inline static float GetListenerZPosition() { return listenerZ; }

		inline static unsigned short int GetCurrentNumberOfSounds() { return totalNumberOfSounds; }

		static float Convert2DTo3DMinDistance(float minDistance2d);

		static float Convert3DTo2DMinDistance(float minDistance3d);

		inline static unsigned short int GetMaxNumberOfSounds() { return maxNumberOfSounds; }

	private:
		// Calls either the soundFunction or musicFunction depending on which dictionary the audioId has been registered in
		// throws if audioId could not be found
		template <typename TReturnType>
		TReturnType ApplyAudioFunction(ID audioId, std::function<TReturnType(sf::Sound &)> soundFunction, std::function<TReturnType(sf::Music &)> musicFunction);

		

	private:
		const SoundBufferHolder<AudioData::ID> & soundHolder;
		const FilePathDictionary & musicFilePathDictionary;

		std::unordered_map<ID, sf::Sound> soundDictionary;
		std::unordered_map<ID, std::unique_ptr<sf::Music>> musicDictionary;

		// Stores the sound buffer id / file path id for each active sf::Sound / sf::Music instance
		std::unordered_map<ID, AudioData::ID> audioDataIdDictionary;

		static float listenerZ;

		static unsigned short int totalNumberOfSounds;
		static const unsigned short int maxNumberOfSounds;
	};

} // namespace mbe