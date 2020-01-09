#pragma once

/// @file
/// @brief class mbe::TiledRenderComponentSerialiser

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TiledRenderComponent
	/// @details XML formal
	/// @code
	/// <Component type="TiledRenderComponent">
	///		<Created>bool</Created>
	///		<Size x="unsigned int" y="unsigned int" />
	///		<TileSize x="unsigend int" y="unsigned int" />
	/// </Component>
	/// @endcode
	class TiledRenderComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TiledRenderComponentSerialiser> Ptr;
		typedef std::weak_ptr<TiledRenderComponentSerialiser> WPtr;
		typedef std::unique_ptr<TiledRenderComponentSerialiser> UPtr;

	public:
		TiledRenderComponentSerialiser() = default;
		~TiledRenderComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

	};


} // namespace mbe