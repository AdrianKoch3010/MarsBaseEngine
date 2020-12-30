#pragma once

/// @file
/// @brief Class mbe::SpriteRenderComponentSerialiser

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>


namespace mbe
{

	/// @brief Component serialiser for the mbe::SpriteRenderComponent
	/// @details Defaults are the same as for the mbe::SpriteRenderComponent
	/// @n@n **XML formal**
	/// @code
	/// <Component type="SpriteRenderComponent">
	///		<!-- optional - default: all 255 -->
	///		<Colour>
	///			<R>unsigned int</R>
	///			<G>unsigned int</G>
	///			<B>unsigned int</B>
	///			<A>unsigned int</A>
	///		</Colour>
	/// </Component>
	/// @endcode
	class SpriteRenderComponentSerialiser : public ComponentSerialiser
	{
	public:
		typedef std::shared_ptr<SpriteRenderComponentSerialiser> Ptr;
		typedef std::weak_ptr<SpriteRenderComponentSerialiser> WPtr;
		typedef std::unique_ptr<SpriteRenderComponentSerialiser> UPtr;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const override;

	};

} // namespace mbe