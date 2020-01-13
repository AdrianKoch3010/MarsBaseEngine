#pragma once

/// @file
/// @brief Class mbe::TextureComponent

#include <cassert>
#include <string>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Graphics/TextureWrapper.h>


//namespace mbe
//{
//
//	// The render system sets the texture wrapper given the texture wrapper id given its texture wrapper component
//	class TextureWrapperComponent : public Component
//	{
//		// For the Set texture wrapper function
//		friend class RenderSystem;
//
//	public:
//		typedef std::shared_ptr<TextureWrapperComponent> Ptr;
//		typedef std::weak_ptr<TextureWrapperComponent> WPtr;
//		typedef std::unique_ptr<TextureWrapperComponent> UPtr;
//
//	public:
//		TextureWrapperComponent(EventManager & eventManager, Entity & parentEntity, const std::string& textureWrapperId);
//		~TextureWrapperComponent() = default;
//
//	public:
//		// Raises mbe::event::ComponentValueChangedEvent (value = textureWrapper)
//		void SetTextureWrapper(const std::string & textureWrapperId, bool resetTextureRect = true);
//
//		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
//		void SetTextureRect(const sf::IntRect & textureRect);
//
//		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
//		void SetTextureRect(sf::IntRect && textureRect);
//
//		// Throws if no texture wrapper is assigned
//		const TextureWrapper& GetTextureWrapper() const;
//
//		inline const std::string& GetTextureWrapperID() const { return textureWrapperId; }
//
//		//inline const TextureWrapperHolder<std::string>& GetTextureWrapperHolder() const { return textureWrapperHolder; }
//
//		inline const sf::IntRect & GetTextureRect() const { return textureRect; }
//
//	private:
//		// This should only be called by the render system
//		inline void SetTextureWrapper(TextureWrapper* textureWrapper) { this->textureWrapper = textureWrapper; }
//
//	private:
//		TextureWrapper* textureWrapper;
//		std::string textureWrapperId;
//		sf::IntRect textureRect;
//	};
//
//} // namespace mbe


namespace mbe
{

	// The render system sets the texture wrapper given the texture wrapper id given its texture wrapper component
	// Allows for adding multiple textures and swapping between them
	// At least one texture must be added. It becomes selected by default
	// There are guaranteed to exist textures up to and including the highest texture id
	class TextureWrapperComponent : public Component
	{
		// For the Set texture wrapper function
		friend class RenderSystem;

	private:
		struct Texture
		{
			TextureWrapper* textureWrapper;
			std::string textureWrapperId;
			sf::IntRect textureRect;
		};

	public:
		typedef std::shared_ptr<TextureWrapperComponent> Ptr;
		typedef std::weak_ptr<TextureWrapperComponent> WPtr;
		typedef std::unique_ptr<TextureWrapperComponent> UPtr;

		typedef unsigned int TextureID;

	private:
		// The id for the specific instance of the texture in this component
		typedef std::unordered_map<TextureID, Texture> TextureDictionary;

	public:
		TextureWrapperComponent(EventManager& eventManager, Entity& parentEntity, const std::string& textureWrapperId);
		~TextureWrapperComponent() = default;

	public:
		// Only adds a texture
		// To later change its texture wrapper and texture rect use the SetTextureWrapper() and SetTextureRect()
		// To make it active use SetActiveTexture
		TextureID AddTexture(const std::string& textureWrapperId);

		// Makes the texture under this id the currently active texture
		// Throws if the id doesn't exist
		// Therefore, use 
		void SetActiveTexture(TextureID textureId);

		// Raises mbe::event::ComponentValueChangedEvent (value = textureWrapper)
		void SetTextureWrapper(const std::string& textureWrapperId, TextureID textureId, bool resetTextureRect = true);
		// Set the currently active texture wrapper id
		void SetTextureWrapper(const std::string& textureWrapperId, bool resetTextureRect = true);

		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
		void SetTextureRect(const sf::IntRect& textureRect, TextureID textureId);
		// Set the currently active texture rect
		void SetTextureRect(const sf::IntRect& textureRect);

		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
		void SetTextureRect(sf::IntRect&& textureRect, TextureID textureId);
		// Set the currently active texture rect
		void SetTextureRect(sf::IntRect&& textureRect);

		// Throws if no texture wrapper is assigned
		const TextureWrapper& GetTextureWrapper(TextureID textureId) const;
		// Get the currently active texture wrapper
		const TextureWrapper& GetTextureWrapper() const;

		const std::string& GetTextureWrapperID(TextureID textureId) const;
		// Get the currently active texture wrapper id
		const std::string& GetTextureWrapperID() const;

		const sf::IntRect& GetTextureRect(TextureID textureId) const;
		// Get the currently active texture rect
		const sf::IntRect& GetTextureRect() const;

		TextureID GetActiveTextureId() const;

		// Returns the number of textures that have been added to this component
		// Use GetHighestTextureID() to get the highest valid id
		size_t GetNumberOfTextures() const;

		// Returns the highest texture id
		// There are guaranteed to exist textures up to and including this id
		// Use GetNumberOfTextures to get the total number of textures added to this component
		TextureID GetHighestTextureID() const;

	private:
		// This should only be called by the render system
		void SetTextureWrapper(TextureWrapper* textureWrapper,  TextureID textureId);
		const TextureDictionary& GetTextureDictionary() const;

		Texture& GetTexture(TextureID textureId);
		const Texture& GetTexture(TextureID textureId) const;

	private:
		TextureID currentTextureId;
		TextureDictionary textureDictionary;
	};

} // namespace mbe