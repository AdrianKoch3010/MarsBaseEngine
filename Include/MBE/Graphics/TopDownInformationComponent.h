#pragma once

/// @file
/// @brief Class mbe::TopDownInformationComponent

#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>

namespace mbe
{

	class TopDownInformationComponent : public Component
	{
	public:
		// Default 0
		TopDownInformationComponent(EventManager& eventManager, Entity& parentEntity, float logicalBottomOffset = 0.f);

		~TopDownInformationComponent() = default;

	public:
		inline float GetLogicalBottomOffset() const { return logicalBottomOffset; }

		inline void SetLogicalBottomOffset(float value) { logicalBottomOffset = value; }

	private:
		float logicalBottomOffset;
	};

} // namespace mbe