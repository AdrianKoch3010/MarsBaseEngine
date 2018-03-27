#include "Effect.h"

using mbe::Effect;

bool Effect::IsSupported()
{
	return sf::Shader::isAvailable();
}

void Effect::ApplyShader(const sf::Shader & shader, sf::RenderTarget & output)
{
	auto outputSize = static_cast<sf::Vector2f>(output.getSize());
	// Maybe increase the output size for the outline shader...

	// Creates two triangles: [0], [1], [2] and [1], [2], [3]
	sf::VertexArray vertices(sf::TrianglesStrip, 4);

	/// Why are the texture coords needed if no texture is drawn? - Nothing is drawn without them
	// Note that the sfml coordinate system's origin is top-left whereas the texture coordinate system's origin is bottom-left
	vertices[0].position = { 0.f, 0.f };
	vertices[0].texCoords = { 0.f, 1.f };

	vertices[1].position = { outputSize.x, 0.f };
	vertices[1].texCoords = { 1.f, 1.f };

	vertices[2].position = { 0.f, outputSize.y };
	vertices[2].texCoords = { 0.f, 0.f };

	vertices[3].position = outputSize;
	vertices[3].texCoords = { 1.f, 0.f };

	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;

	output.draw(vertices, states);
}
