#pragma once

/// @file
/// @brief Class mbe::MusicComponent

#include <string>
#include <unordered_map>

#include <SFML/Audio/Music.hpp>

#include <MBE/Audio/BaseAudioComponent.h>

namespace mbe
{

	class MusicComponent : public BaseAudioComponent
	{
	public:
		typedef std::shared_ptr<MusicComponent> Ptr;
		typedef std::weak_ptr<MusicComponent> WPtr;
		typedef std::unique_ptr<MusicComponent> UPtr;

	public:
		explicit MusicComponent(Entity & parentEntity, std::string filePath);

		~MusicComponent() = default;

	public:
		void Update(sf::Time frameTime) override;

		void Play() override;

		void SetPaused(bool value = true) override;

		void Stop() override;

		void SetAttenuation(float value);

		void SetMinDistance(float value) override;

		// Volume between 0 and 100
		// 0 is mute and 100 is full value
		// The default volume is 100
		void SetVolume(float value) override;

		void SetPitch(float value) override;

		void SetPlayingOffset(sf::Time offset) override;

		void SetLooped(bool value = true) override;

		void SetPosition(sf::Vector2f position) override;

		float GetAttenuation() const override;

		float GetMinDistance() const override;

		// Volume between 0 and 100
		// 0 is mute and 100 is full value
		// The default volume is 100
		float GetVolume() const override;

		float GetPitch() const override;

		sf::Time GetPlayingOffset() const override;

		float IsLooped() const override;

		sf::Vector2f GetPosition() const override;

		// Is stopped valid?
		AudioStatus GetAudioStatus() const override;

	public:
		const std::string & GetFilePath() const;

	private:
		sf::Music music;
		const std::string filePath;
	};

} // namespace mbe