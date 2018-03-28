#pragma once

/// @file
/// @brief Class mbe::SoundComponent

#include <string>
#include <memory>

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Core/Entity.h>
#include <MBE/Core/HandleBase.h>
#include <MBE/Core/Utility.h>
#include <MBE/Audio/AudioData.h>

namespace mbe
{

	// Requires a transform component for the position
	// If the entity has no transform component the audio is non-spatialised
	// Inherits from handle base to enable it being animated
	class AudioComponent : public Component, public HandleBase<AudioComponent>
	{
	public:
		typedef std::shared_ptr<AudioComponent> Ptr;
		typedef std::weak_ptr<AudioComponent> WPtr;
		typedef std::unique_ptr<AudioComponent> UPtr;

	public:
		// Creates a default audio component and only sets the audio source id
		AudioComponent(Entity & parentEntity, AudioData::ID audioSourceId = { "" });

		// Copies the passed in audio data
		AudioComponent(Entity & parentEntity, const AudioData & audioData);

		virtual ~AudioComponent() = default;

	public:
		inline void Update(sf::Time frameTime) override {}

		void SetAudioData(const AudioData & audioData);

		// This function is prefered over SetAudioData() since the data can be modified directly
		AudioData & GetAudioData();

		// const overload
		const AudioData & GetAudioData() const;

	private:
		AudioData audioData;
	};

} // namespace mbe