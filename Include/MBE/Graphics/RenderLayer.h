#pragma once

/// @file
/// @brief Class mbe::detail::RenderLayer

#include <vector>

#include <SFML/Graphics/View.hpp>

#include <MBE/Core/Entity.h>

namespace mbe
{
	namespace detail
	{
		/// @brief The render layer determines in which order and how (using which view/coordinate system) the render objects are drawn
		class RenderLayer
		{
		public:
			RenderLayer() = default;
			~RenderLayer() = default;

			inline void SetView(const sf::View & view) { this->view = view; }

			inline sf::View & GetView() { return view; }
			// Const overload
			inline const sf::View & GetView() const { return view; }

			inline std::vector<Entity::HandleID> & GetRenderEntityIdList() { return renderEntityIdList; }

			void AddRenderEntity(Entity::HandleID entityId);

			// Throws runntime error if the node could not be found
			// If the same node has been added multiple times, it only removes one of them
			void RemoveRenderEntity(Entity::HandleID entityId);

		private:
			sf::View view;
			std::vector<Entity::HandleID> renderEntityIdList;
		};
	} // namespace detail
} // namespace mbe