#pragma once

/// @file
/// @brief Class mbe::TopDownInformationComponent

#include "Entity.h"
#include "Component.h"

namespace mbe
{

	class TopDownInformationComponent : public Component
	{
	public:
		TopDownInformationComponent(Entity & parentEntity, float logicalBottomOffset = 0.f);

		~TopDownInformationComponent() = default;

	public:
		inline void Update(sf::Time frameTime) override {}

		inline void SetLogicalBottomOffset(float value) { logicalBottomOffset = value; }

		inline float GetLogicalBottomOffset() const { return logicalBottomOffset; }

	private:
		float logicalBottomOffset;
	};

} // namespace mbe