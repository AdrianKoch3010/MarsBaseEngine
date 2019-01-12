#pragma once

/// @file
/// @brief Class mbe::BaseAudioComponent

#include <cassert>

#include <SFML/Audio/Listener.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Core/Entity.h>

#include <MBE/TransformComponent.h>
#include <MBE/Graphics/RenderInformationComponent.h>

#include <MBE/Audio/AudioData.h>

namespace mbe
{
	// This class behaves differently depending on whetehr the entity has a transform or / and render component
	class BaseAudioComponent : public Component
	{
	public:
		typedef std::shared_ptr<BaseAudioComponent> Ptr;
		typedef std::weak_ptr<BaseAudioComponent> WPtr;
		typedef std::unique_ptr<BaseAudioComponent> UPtr;

		typedef AudioData::AudioStatus AudioStatus;

	public:
		BaseAudioComponent(Entity & parentEntity);
		virtual ~BaseAudioComponent();

	public:
		virtual void Play() = 0;

		virtual void SetPaused(bool value = true) = 0;

		virtual void Stop() = 0;

		virtual void SetAttenuation(float value) = 0;

		virtual void SetMinDistance(float value) = 0;

		virtual void SetVolume(float value) = 0;

		virtual void SetPitch(float value) = 0;

		virtual void SetPlayingOffset(sf::Time offset) = 0;

		virtual void SetLooped(bool value = true) = 0;

		virtual float GetAttenuation() = 0;

		virtual float GetMinDistance() = 0;

		virtual float GetVolume() = 0;

		virtual float GetPitch() = 0;

		virtual sf::Time GetPlayingOffset() = 0;

		virtual float IsLooped() = 0;

		virtual AudioStatus GetAudioStatus() = 0;

	protected:
		// Logic that depends on Audio, Transform and RenderInformation / View Components
		sf::Vector2f CalculatePosition();
	};

} // namespace mbe