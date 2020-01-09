#pragma once

/// @file
/// @brief Class mbe::TileComponent

#include <vector>
#include <memory>

#include <MBE/Core/Component.h>

namespace mbe
{

	class TileComponent : public Component
	{
	public:
		TileComponent(EventManager & eventManager, Entity & parentEntity);

		~TileComponent() = default;

	public:
		inline void SetIndexList(const std::vector<size_t> & indexList) { this->indexList = indexList; }

		inline void SetIndexList(std::vector<size_t> && indexList) { this->indexList = std::move(indexList); }

		inline const std::vector<size_t> & GetIndexList() const { return indexList; }

	private:
		std::vector<size_t> indexList;

	};

} // namespace mbe