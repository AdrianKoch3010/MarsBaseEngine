#pragma once

/// @file
/// @brief class mbe::TextureWrapperComponentSerialiser

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TextureWrapperComponent
	/// @details XML format
	/// @code
	/// <Component type="TextureWrapperComponent">
	///
	/// </Component>
	/// @endcode
	class TextureWrapperComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TextureWrapperComponentSerialiser> Ptr;
		typedef std::weak_ptr<TextureWrapperComponentSerialiser> WPtr;
		typedef std::unique_ptr<TextureWrapperComponentSerialiser> UPtr;

	public:
		TextureWrapperComponentSerialiser() = default;
		~TextureWrapperComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;

	};


} // namespace mbe