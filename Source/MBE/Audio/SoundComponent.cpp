#include <Mbe/Audio/SoundComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(SoundComponent, BaseAudioComponent)

SoundComponent::SoundComponent(Entity & parentEntity, const sf::SoundBuffer & soundBuffer) :
	BaseAudioComponent(parentEntity),
	soundBuffer(soundBuffer),
	sound(soundBuffer)
{
}

void SoundComponent::Update(sf::Time frameTime)
{
	// Delete the sound entity once the sound has finished playing
	if (sound.getStatus() == sf::Sound::Status::Stopped)
		this->parentEntity.Destroy();
}

void SoundComponent::Play()
{
	sound.setBuffer(soundBuffer);
	sound.play();
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
	sound.setMinDistance(AudioData::Convert3DTo2DMinDistance(value));
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

void SoundComponent::SetPosition(sf::Vector2f position)
{
	sound.setPosition({ position.x, position.y, 0.f });
}

float SoundComponent::GetAttenuation() const
{
	return sound.getAttenuation();
}

float SoundComponent::GetMinDistance() const
{
	return AudioData::Convert3DTo2DMinDistance(sound.getMinDistance());
}

float SoundComponent::GetVolume() const
{
	return sound.getVolume();
}

float SoundComponent::GetPitch() const
{
	return sound.getPitch();
}

sf::Time SoundComponent::GetPlayingOffset() const
{
	return sound.getPlayingOffset();
}

float SoundComponent::IsLooped() const
{
	return sound.getLoop();
}

sf::Vector2f SoundComponent::GetPosition() const
{
	return { sound.getPosition().x, sound.getPosition().y };
}

AudioData::AudioStatus SoundComponent::GetAudioStatus() const
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
