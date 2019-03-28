#include <SFML/System/Vector2.hpp>

#include <MBE/Core/Entity.h>
#include <MBE/Core/EntityManager.h>

#include <MBE/Audio/BaseAudioComponent.h>
#include <MBE/TransformComponent.h>
#include <MBE/Graphics/RenderInformationComponent.h>

#include <MBE/Audio/AudioData.h>

namespace mbe
{

	class AudioSystem
	{
	public:
		AudioSystem(const EntityManager & entityManager);
		~AudioSystem() = default;

	public:
		void Update();

	private:
		// Logic that depends on Audio, Transform and RenderInformation / View Components
		sf::Vector2f CalculatePosition(const Entity & entity);

	private:
		const EntityManager & entityManager;
	};

} // namespace mbe