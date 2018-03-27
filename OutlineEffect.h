#pragma once

#include <string>
#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>

#include "AssetHolder.h"
#include "Effect.h"

namespace mbe
{
	class OutlineEffect : public Effect
	{
	private:
		//typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

	public:
		OutlineEffect();
		~OutlineEffect() = default;

	public:
		//void Apply(const sf::RenderTexture & input, sf::RenderTarget & output) override;
		sf::RenderTexture & Apply(const sf::Texture & texture) override;

	private:
		void PrepareTextures(sf::Vector2u size);

		//void Outline(const sf::RenderTexture & input, sf::RenderTexture & output);
		//void Blur(const sf::RenderTexture & input, sf::RenderTexture & output);
		//void Add(const sf::RenderTexture & source, const sf::RenderTexture & blurredOutline, sf::RenderTarget & output);

	private:
		ShaderHolder<std::string> shaderHolder;

		sf::RenderTexture bigTexture;
		sf::RenderTexture outlineTexture;
		sf::RenderTexture blurredOutlineTexture;
	};
} // namespace mbe