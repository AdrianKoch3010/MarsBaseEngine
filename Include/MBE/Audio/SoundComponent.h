#pragma once

/// @file
/// @brief Class mbe::SoundComponent

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <MBE/Audio/BaseAudioComponent.h>

namespace mbe
{

	class SoundComponent : public BaseAudioComponent
	{
	public:
		typedef std::shared_ptr<SoundComponent> Ptr;
		typedef std::weak_ptr<SoundComponent> WPtr;
		typedef std::unique_ptr<SoundComponent> UPtr;

	public:
		SoundComponent(Entity & parentEntity, const sf::SoundBuffer & soundBuffer);

		~SoundComponent() = default;

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
		const sf::SoundBuffer & GetSoundBuffer() const;

	private:
		sf::Sound sound;
		const sf::SoundBuffer & soundBuffer;
	};

} // namespace mbe


/// How to deal with the differnt ways to get the position