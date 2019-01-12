//#include <MBE/Audio/AudioData.h>
//
//using namespace mbe;
//
//AudioData AudioData::Default = AudioData(ID(), AudioData::Type::Sound, 8.f, 200.f, 100.f, 1.f, sf::seconds(0.f), false);
//
//AudioData::AudioData(ID soundSourceId, AudioData::Type audioType, float attenuation, float minDistance, float volume, float pitch, sf::Time playingOffset, bool looped) :
//	audioSourceId(soundSourceId),
//	audioType(audioType),
//	attenuation(attenuation),
//	minDistance(minDistance),
//	volume(volume),
//	pitch(pitch),
//	playingOffset(playingOffset),
//	looped(looped)
//{
//	NormaliseIDString(soundSourceId);
//}
//
//void AudioData::SetAudioSourceID(ID soundSourceId)
//{
//	NormaliseIDString(soundSourceId);
//	this->audioSourceId = std::move(soundSourceId);
//}

#include <MBE/Audio/AudioData.h>

using namespace mbe;

float AudioData::listenerZ = 300.f;

unsigned short int AudioData::totalNumberOfSounds = 0u;
const unsigned short int AudioData::maxNumberOfSounds = 255u; // Internal limit set by SFML / OpenAL

void AudioData::SetListenerPosition(const sf::Vector2f & position)
{
	sf::Listener::setPosition(position.x, -position.y, listenerZ);
}

sf::Vector2f AudioData::GetListenerPosition()
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

float AudioData::Convert2DTo3DMinDistance(float minDistance2d)
{
	// Pythagoras
	return std::sqrt(std::pow(minDistance2d, 2) + std::pow(listenerZ, 2));
}

float AudioData::Convert3DTo2DMinDistance(float minDistance3d)
{
	// Inverse function of Convert2DTo3DMinDistance
	return std::sqrt(std::pow(minDistance3d, 2) - std::pow(listenerZ, 2));
}
