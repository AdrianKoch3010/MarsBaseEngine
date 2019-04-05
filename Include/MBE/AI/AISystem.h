#pragma once

#include <MBE/Core/EntityManager.h>
#include <MBE/AI/AIComponent.h>

namespace mbe
{

	class AISystem
	{
	public:
		AISystem(EntityManager & entityManager);
		~AISystem() = default;

	public:
		void Update();

	private:
		EntityManager & entityManager;

	};

} // namespace mbe