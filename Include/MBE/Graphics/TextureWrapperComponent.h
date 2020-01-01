#pragma once

/// @file
/// @brief Class mbe::TextureComponent

#include <cassert>
#include <string>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Graphics/TextureWrapper.h>


namespace mbe
{

	// The render system sets the texture wrapper given the texture wrapper id given its texture wrapper component
	class TextureWrapperComponent : public Component
	{
		// For the Set texture wrapper function
		friend class RenderSystem;

	public:
		typedef std::shared_ptr<TextureWrapperComponent> Ptr;
		typedef std::weak_ptr<TextureWrapperComponent> WPtr;
		typedef std::unique_ptr<TextureWrapperComponent> UPtr;

	public:
		TextureWrapperComponent(EventManager & eventManager, Entity & parentEntity, const std::string& textureWrapperName);
		~TextureWrapperComponent() = default;

	public:
		// Raises mbe::event::ComponentValueChangedEvent (value = textureWrapper)
		void SetTextureWrapper(const std::string & textureWrapperId, bool resetTextureRect = true);

		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
		void SetTextureRect(const sf::IntRect & textureRect);

		// Raises mbe::event::ComponentValueChangedEvent (value = textureRect)
		void SetTextureRect(sf::IntRect && textureRect);

		// Throws if no texture wrapper is assigned
		const TextureWrapper& GetTextureWrapper() const;

		inline const std::string& GetTextureWrapperID() const { return textureWrapperId; }

		//inline const TextureWrapperHolder<std::string>& GetTextureWrapperHolder() const { return textureWrapperHolder; }

		inline const sf::IntRect & GetTextureRect() const { return textureRect; }

	private:
		// This should only be called by the render system
		inline void SetTextureWrapper(TextureWrapper* textureWrapper) { this->textureWrapper = textureWrapper; }

	private:
		//const TextureWrapperHolder<std::string>& textureWrapperHolder;
		TextureWrapper* textureWrapper;
		std::string textureWrapperId;
		sf::IntRect textureRect;
	};

} // namespace mbe