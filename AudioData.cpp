#include "AudioData.h"

using namespace mbe;

AudioData AudioData::Default = AudioData(ID(), AudioData::Type::Sound, 8.f, 200.f, 100.f, 1.f, sf::seconds(0.f), false);

AudioData::AudioData(ID soundSourceId, AudioData::Type audioType, float attenuation, float minDistance, float volume, float pitch, sf::Time playingOffset, bool looped) :
	audioSourceId(soundSourceId),
	audioType(audioType),
	attenuation(attenuation),
	minDistance(minDistance),
	volume(volume),
	pitch(pitch),
	playingOffset(playingOffset),
	looped(looped)
{
	NormaliseIDString(soundSourceId);
}

void AudioData::SetAudioSourceID(ID soundSourceId)
{
	NormaliseIDString(soundSourceId);
	this->audioSourceId = std::move(soundSourceId);
}
