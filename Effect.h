#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>

// Should effect inherit from HandleBase?
#include "HandleBase.h"

namespace mbe
{
	class Effect : public HandleBase<Effect>, private sf::NonCopyable
	{
	public:
		Effect() = default;
		virtual ~Effect() = default;

		//virtual void Apply(const sf::RenderTexture & input, sf::RenderTarget & output) = 0;
		virtual sf::RenderTexture & Apply(const sf::Texture & texture) = 0;

		static bool IsSupported();

	protected:
		static void ApplyShader(const sf::Shader & shader, sf::RenderTarget & output);
	};

} //namespace mbe