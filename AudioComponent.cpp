#include "AudioComponent.h"

using namespace mbe;

AudioComponent::AudioComponent(Entity & parentEntity, AudioData::ID audioSourceId) :
	Component(parentEntity),
	audioData(AudioData::Default)
{
	audioData.SetAudioSourceID(audioSourceId);
}

AudioComponent::AudioComponent(Entity & parentEntity, const AudioData & audioData) :
	Component(parentEntity),
	audioData(audioData)
{
}

void AudioComponent::SetAudioData(const AudioData & audioData)
{
	this->audioData = audioData;
}

AudioData & AudioComponent::GetAudioData()
{
	return audioData;
}

const AudioData & AudioComponent::GetAudioData() const
{
	return audioData;
}
