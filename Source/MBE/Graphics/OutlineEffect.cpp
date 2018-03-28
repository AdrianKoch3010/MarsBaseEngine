#include <MBE/Graphics/OutlineEffect.h>

using mbe::OutlineEffect;

OutlineEffect::OutlineEffect()
{
	/// Just for testing purposes
	// Load the shaders
	shaderHolder.Load("Outline", "Resources/Shaders/OutlineShader.txt", sf::Shader::Fragment);
	shaderHolder.Load("Blur", "Resources/Shaders/BlurShader.txt", sf::Shader::Fragment);
	//shaderHolder.Load("Add", "Resources/Shaders/AddShader.txt", sf::Shader::Fragment);

	sf::Shader & outlineShader = shaderHolder["Outline"];
	// Set parameters/uniforms
	//outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
	outlineShader.setUniform("outlineThickness", 10.f);
	outlineShader.setUniform("outlineColor", sf::Vector3f(98.0f / 255.0f, 221.0f / 255.0f, 15.0f / 255.0f)); // green ish

	//sf::Shader & blurShader = shaderHolder["Blur"];
	// Set parameters/uniforms
	//blurShader.setUniform("texture", sf::Shader::CurrentTexture);
}

sf::RenderTexture & OutlineEffect::Apply(const sf::Texture & texture)
{
	//PrepareTextures(input.getSize());

	//Outline(input, outlineTexture);
	//Blur(outlineTexture, blurTexture);

	//output.draw(sf::Sprite(input.getTexture()));
	//output.draw(sf::Sprite(blurTexture.getTexture()));
	////Add(input, blurTexture, output);

	// Create all the renderTextures elsewhere
	/// Since they must be a different size for each drawable, it might make sense to create an effect PER drawable
	//sf::Texture texture;
	//sf::RenderTexture bigTexture;
	//sf::RenderTexture outlineTexture;
	//sf::RenderTexture blurredOutlineTexture;


	//const sf::Vector2u additionalSize = sf::Vector2u(100, 100); // later a percentage of the texture size

	PrepareTextures(/*additionalSize + */texture.getSize());

	// Draw the texture on the bigger texture 
	/*sf::Sprite sprite(texture);
	sprite.setPosition(sf::Vector2f(additionalSize) * 0.5f);
	bigTexture.draw(sprite);
	bigTexture.display();*/

	// Draw the outline texture using the outline shader
	sf::Shader & outlineShader = shaderHolder["Outline"];
	outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
	outlineShader.setUniform("textureSize", sf::Vector2f(/*additionalSize + */texture.getSize()));
	outlineTexture.draw(sf::Sprite(/*bigTexture.getTexture()*/texture), &outlineShader);
	outlineTexture.display();

	// Draw the blurred outline texture using the blur shader
	sf::Shader & blurShader = shaderHolder["Blur"];
	blurShader.setUniform("texture", sf::Shader::CurrentTexture);
	blurShader.setUniform("blur_radius", 3.f / /*additionalSize.x + */texture.getSize().x); // later maybe square root of x^2 + y^2
	blurredOutlineTexture.draw(sf::Sprite(outlineTexture.getTexture()), &blurShader);
	blurredOutlineTexture.display();

	//std::shared_ptr<sf::Texture> texturePtr = std::make_shared<sf::Texture>(blurredOutlineTexture.getTexture()); // How expensive is this?
	return blurredOutlineTexture;
}

#include <SFML/System/Clock.hpp>
#include <iostream>

void OutlineEffect::PrepareTextures(sf::Vector2u size)
{
	// meassure the time it takes to create the textures
	sf::Clock clock;
	sf::Time time = sf::Time::Zero;
	clock.restart();

	if (bigTexture.getSize() != size || outlineTexture.getSize() != size || blurredOutlineTexture.getSize() != size)
	{
		// Set smooth?
		bigTexture.create(size.x, size.y);
		bigTexture.setSmooth(true);

		outlineTexture.create(size.x, size.y);
		outlineTexture.setSmooth(true);

		blurredOutlineTexture.create(size.x, size.y);
		blurredOutlineTexture.setSmooth(true);

		time = clock.restart();
		std::cout << std::endl << "Creating the render textures took: " << time.asMicroseconds() << " microseconds (" << time.asSeconds() << ")";
	}

	bigTexture.clear(sf::Color::Transparent);
	outlineTexture.clear(sf::Color::Transparent);
	blurredOutlineTexture.clear(sf::Color::Transparent);
}

//void OutlineEffect::Outline(const sf::RenderTexture & input, sf::RenderTexture & output)
//{
//	sf::Shader & outlineShader = shaderHolder["Outline"];
//	
//	// Maybe there is a need to clear the texture
//	//output.clear(sf::Color::Transparent);
//
//	/// Make the parameters variable
//	// Set parameters/uniforms
//	outlineShader.setUniform("texture", input.getTexture());
//	outlineShader.setUniform("outlineThickness", 10.f);
//	outlineShader.setUniform("textureSize", sf::Vector2f(input.getSize().x, input.getSize().y));
//	outlineShader.setUniform("outlineColor", sf::Vector3f(98.0f / 255.0f, 221.0f / 255.0f, 15.0f / 255.0f)); // green ish
//
//	//ApplyShader(outlineShader, output);
//	output.draw(sf::Sprite(input.getTexture()), &outlineShader);
//	output.display();
//}
//
//void mbe::OutlineEffect::Blur(const sf::RenderTexture & input, sf::RenderTexture & output)
//{
//	sf::Shader & blurShader = shaderHolder["Blur"];
//
//	// Maybe there is a need to clear the texture
//	//output.clear(sf::Color::Transparent);
//
//	/// Make the parameters variable
//	// Set parameters/uniforms
//	blurShader.setUniform("texture", input.getTexture());
//	blurShader.setUniform("blur_radius", 3.f / input.getSize().x + 100);
//
//	output.draw(sf::Sprite(input.getTexture()), &blurShader);
//	//ApplyShader(blurShader, output);
//	output.display();
//}
//
//void OutlineEffect::Add(const sf::RenderTexture & source, const sf::RenderTexture & blurredOutline, sf::RenderTarget & output)
//{
//	sf::Shader & addShader = shaderHolder["Add"];
//
//	addShader.setUniform("source", source.getTexture());
//	addShader.setUniform("outline", blurredOutline.getTexture());
//
//	ApplyShader(addShader, output);
//	//output.draw(sf::Sprite(source.getTexture()))
//}
