#pragma once

/// @file
/// @brief Class mbe::TextureComponent

#include <cassert>
#include <string>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include <MBE/Core/Component.h>
#include <MBE/Graphics/TextureWrapper.h>
#include <MBE/Core/AssetHolder.h>


namespace mbe
{

	class TextureWrapperComponent : public Component
	{
	public:
		typedef std::shared_ptr<TextureWrapperComponent> Ptr;
		typedef std::weak_ptr<TextureWrapperComponent> WPtr;
		typedef std::unique_ptr<TextureWrapperComponent> UPtr;

	public:
		TextureWrapperComponent(EventManager & eventManager, Entity & parentEntity, const TextureWrapperHolder<std::string>& textureWrapperHolder, const std::string& textureWrapperName);
		~TextureWrapperComponent() = default;

	public:
		void SetTextureWrapper(const std::string & textureWrapperName, bool resetTextureRect = true);

		void SetTextureRect(const sf::IntRect & textureRect);

		void SetTextureRect(sf::IntRect && textureRect);

		const TextureWrapper& GetTextureWrapper() const;

		inline const std::string& GetTextureWrapperName() const { return textureWrapperName; }

		inline const TextureWrapperHolder<std::string>& GetTextureWrapperHolder() const { return textureWrapperHolder; }

		inline const sf::IntRect & GetTextureRect() const { return textureRect; }

	private:
		const TextureWrapperHolder<std::string>& textureWrapperHolder;
		std::string textureWrapperName;
		sf::IntRect textureRect;
	};

} // namespace mbe