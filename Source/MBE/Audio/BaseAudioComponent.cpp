#include <MBE/Audio/BaseAudioComponent.h>

using namespace mbe;

BaseAudioComponent::BaseAudioComponent(Entity & parentEntity) : Component(parentEntity)
{
	AudioData::IncrementNumberOfSounds();
}

BaseAudioComponent::~BaseAudioComponent()
{
	AudioData::DecrementNumberOfSounds();
}