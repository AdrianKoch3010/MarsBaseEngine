#include <MBE/Audio/MusicComponent.h>

using namespace mbe;

MBE_ENABLE_COMPONENT_POLYMORPHISM(MusicComponent, BaseAudioComponent)

MusicComponent::MusicComponent(Entity & parentEntity, std::string filePath) :
	BaseAudioComponent(parentEntity),
	filePath(filePath)
{
}

void MusicComponent::Update(sf::Time frameTime)
{
	// Delete the sound entity once the sound has finished playing
	if (music.getStatus() == sf::Music::Status::Stopped)
		this->parentEntity.Destroy();
}

void MusicComponent::Play()
{
	// Open the file path
	music.openFromFile(filePath);

	// Play the sound
	music.play();
}

void MusicComponent::SetPaused(bool value)
{
	if (value == true)
	{
		if (music.getStatus() == sf::Music::Status::Playing)
			music.pause();
	}
	else
	{
		if (music.getStatus() == sf::Music::Status::Paused)
			music.play();
	}
}

void MusicComponent::Stop()
{
	// Stop the sound
	music.stop();

	// Destroy the parent entity
	parentEntity.Destroy();
}

void MusicComponent::SetAttenuation(float value)
{
	music.setAttenuation(value);
}

void MusicComponent::SetMinDistance(float value)
{
	music.setMinDistance(AudioData::Convert3DTo2DMinDistance(value));
}

void MusicComponent::SetVolume(float value)
{
	music.setVolume(value);
}

void MusicComponent::SetPitch(float value)
{
	music.setPitch(value);
}

void MusicComponent::SetPlayingOffset(sf::Time offset)
{
	music.setPlayingOffset(offset);
}

void MusicComponent::SetLooped(bool value)
{
	music.setLoop(value);
}

void MusicComponent::SetPosition(sf::Vector2f position)
{
	music.setPosition({ position.x, position.y, 0.f });
}

float MusicComponent::GetAttenuation() const
{
	return music.getAttenuation();
}

float MusicComponent::GetMinDistance() const
{
	return AudioData::Convert3DTo2DMinDistance(music.getMinDistance());
}

float MusicComponent::GetVolume() const
{
	return music.getVolume();
}

float MusicComponent::GetPitch() const
{
	return music.getPitch();
}

sf::Time MusicComponent::GetPlayingOffset() const
{
	return music.getPlayingOffset();
}

float MusicComponent::IsLooped() const
{
	return music.getLoop();
}

sf::Vector2f MusicComponent::GetPosition() const
{
	return { music.getPosition().x, music.getPosition().y };
}

AudioData::AudioStatus MusicComponent::GetAudioStatus() const
{
	switch (music.getStatus())
	{
	case sf::Music::Status::Playing:
		return AudioStatus::Playing;
	case sf::Music::Status::Paused:
		return AudioStatus::Paused;
	default:
		return AudioStatus::Stopped;
	}
}

const std::string & MusicComponent::GetFilePath() const
{
	return filePath;
}
