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
		// All rows must be the same length
		typedef std::vector<std::vector<float>> MovementSpeedShape;

	public:
		TileMapComponent(EventManager& eventManager, Entity& parentEntity);
		~TileMapComponent() = default;

		// Default is { 0u, 0u }
		inline const Position& GetPosition() const { return position; }

		// Default is an empty vector
		inline const MovementSpeedShape& GetMovementSpeedShape() const { return movementSpeedShape; }

		// Throw if not in range
		float GetMovementSpeed(unsigned int x, unsigned int y) const;

		sf::Vector2u GetSize() const;

		inline void SetPosition(const Position& position) { this->position = position; }

		inline void SetMovementSpeedShape(const MovementSpeedShape& movementSpeedShape) { this->movementSpeedShape = movementSpeedShape; }
		inline void SetMovementSpeedShape(MovementSpeedShape&& movementSpeedShape) { this->movementSpeedShape = std::move(movementSpeedShape); }

		// Throw if not in range
		void SetMovementSpeed(unsigned int x, unsigned int y, float speed);

		// Makes the rows columns and vice versa
		void Rotate();

	private:
		Position position;
		MovementSpeedShape movementSpeedShape;
	};

} // namespace mbe