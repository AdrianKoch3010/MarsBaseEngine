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
		SoundComponent(Entity & parentEntity);

		~SoundComponent() = default;

	public:

		void Update(sf::Time frameTime) override;

		void Play() override;

		void SetPaused(bool value = true) override;

		void Stop() override;

		void SetAttenuation(float value);

		void SetMinDistance(float value) override;

		void SetVolume(float value) override;

		void SetPitch(float value) override;

		void SetPlayingOffset(sf::Time offset) override;

		void SetLooped(bool value = true) override;

		float GetAttenuation() override;

		float GetMinDistance() override;

		float GetVolume() override;

		float GetPitch() override;
		
		sf::Time GetPlayingOffset() override;
		
		float IsLooped() override;
		
		// Is stopped valid?
		AudioStatus GetAudioStatus() override;

	private:
		sf::Sound sound;
	};

} // namespace mbe


/// How to deal with the differnt ways to get the position