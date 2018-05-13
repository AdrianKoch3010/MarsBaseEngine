#include <Mbe/Audio/SoundComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(SoundComponent, BaseAudioComponent)

SoundComponent::SoundComponent(Entity & parentEntity, const sf::SoundBuffer & soundBuffer) :
	BaseAudioComponent(parentEntity),
	soundBuffer(soundBuffer)
{
}

void SoundComponent::Update(sf::Time frameTime)
{
	// Update the sound position
	const auto position = CalculatePosition();
	sound.setPosition({ position.x, position.y, 0.f });

	// Delete the sound entity once the sound has finished playing
	if (sound.getStatus() == sf::Sound::Status::Stopped)
		this->parentEntity.Destroy();
}

void SoundComponent::Play()
{
	sound.setBuffer(soundBuffer);
	sound.play();

	// Set its initial position
	const auto position = CalculatePosition();
	sound.setPosition({ position.x, position.y, 0.f });
}

void SoundComponent::SetPaused(bool value)
{
	if (value == true)
	{
		if (sound.getStatus() == sf::Sound::Status::Playing)
			sound.pause();
	}
	else
	{
		if (sound.getStatus() == sf::Sound::Status::Paused)
			sound.play();
	}
}

void SoundComponent::Stop()
{
	// Stop the sound
	sound.stop();

	// Destroy the parent entity
	parentEntity.Destroy();
}

void SoundComponent::SetAttenuation(float value)
{
	sound.setAttenuation(value);
}

void SoundComponent::SetMinDistance(float value)
{
	sound.setMinDistance(Convert3DTo2DMinDistance(value));
}

void SoundComponent::SetVolume(float value)
{
	sound.setVolume(value);
}

void SoundComponent::SetPitch(float value)
{
	sound.setPitch(value);
}

void SoundComponent::SetPlayingOffset(sf::Time offset)
{
	sound.setPlayingOffset(offset);
}

void SoundComponent::SetLooped(bool value)
{
	sound.setLoop(value);
}

float SoundComponent::GetAttenuation()
{
	return sound.getAttenuation();
}

float SoundComponent::GetMinDistance()
{
	return Convert3DTo2DMinDistance(sound.getMinDistance());
}

float SoundComponent::GetVolume()
{
	return sound.getVolume();
}

float SoundComponent::GetPitch()
{
	return sound.getPitch();
}

sf::Time SoundComponent::GetPlayingOffset()
{
	return sound.getPlayingOffset();
}

float SoundComponent::IsLooped()
{
	return sound.getLoop();
}

BaseAudioComponent::AudioStatus SoundComponent::GetAudioStatus()
{
	switch (sound.getStatus())
	{
	case sf::Sound::Status::Playing:
		return AudioStatus::Playing;
	case sf::Sound::Status::Paused:
		return AudioStatus::Paused;
	default:
		return AudioStatus::Stopped;
	}
}

const sf::SoundBuffer & SoundComponent::GetSoundBuffer() const
{
	return *sound.getBuffer();
}
