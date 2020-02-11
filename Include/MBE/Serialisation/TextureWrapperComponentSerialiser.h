#pragma once

/// @file
/// @brief class mbe::TextureWrapperComponentSerialiser

#include <memory>

#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Core/AssetHolder.h>

namespace mbe
{

	/// @brief Component serialiser for the mbe::TextureWrapperComponent
	/// @details **XML format**
	/// @code
	/// <Component type="TextureWrapperComponent">
	///		<ActiveTexture>unsigned int<ActiveTexture>
	///		<Textures>
	///			<Texture>
	///				<!-- optional TextureRect -->
	///				<TextureRect>
	///					<Top>int</Top>
	///					<Left>int</Left>
	///					<Width>int</Width>
	///					<Height>int</Height>
	///				</TextureRect>
	///				<TextureWrapper>string</TextureWrapper>
	///			</Texture>
	///			...
	///		</Textures>
	/// </Component>
	/// @endcode
	class TextureWrapperComponentSerialiser : public ComponentSerialser
	{
	public:
		typedef std::shared_ptr<TextureWrapperComponentSerialiser> Ptr;
		typedef std::weak_ptr<TextureWrapperComponentSerialiser> WPtr;
		typedef std::unique_ptr<TextureWrapperComponentSerialiser> UPtr;

	public:
		/// @brief Constructor
		TextureWrapperComponentSerialiser();

		/// @brief Default destructor
		~TextureWrapperComponentSerialiser() = default;

	public:
		void LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) override;

		void StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;
	};


} // namespace mbe