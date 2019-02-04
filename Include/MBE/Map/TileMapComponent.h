#pragma once

/// @file
/// @brief Class mbe::TileMapComponent

#include <memory>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Map/TileMapBase.h>


namespace mbe
{

	class TileMapComponent : public Component
	{
	public:
		typedef std::shared_ptr<TileMapComponent> Ptr;
		typedef std::weak_ptr<TileMapComponent> WPtr;
		typedef std::unique_ptr<TileMapComponent> UPtr;

		typedef typename TileMapBase::Position Position;
		
		// Movement speed per tile and shape from top to bottom
		// E.g. { { 1.f 1.f }, { 1.f, 5.f 1.f }, { 3.f } }
		// [1] [1]
		// [1] [5] [1]
		// [3]
		typedef std::vector<std::vector<float>> MovementSpeedShape;

	public:
		TileMapComponent(Entity & parentEntity);
		~TileMapComponent() = default;

		inline const Position & GetPosition() const { return position; }

		inline const MovementSpeedShape & GetMovementSpeedShape() const { return movementSpeedShape; }

		// Throw if not in range
		float GetMovementSpeed(unsigned int x, unsigned int y) const;

		inline void SetPosition(const Position & position) { this->position = position; }

		inline void SetMovementSpeedShape(const MovementSpeedShape & movementSpeedShape) { this->movementSpeedShape = movementSpeedShape; }

		inline void SetMovementSpeedShape(MovementSpeedShape && movementSpeedShape) { this->movementSpeedShape = std::move(movementSpeedShape); }

		// Throw if not in range
		void SetMovementSpeed(unsigned int x, unsigned int y, float speed);

	private:
		Position position;
		MovementSpeedShape movementSpeedShape;
	};

} // namespace mbe