#include <MBE/Core/PixelMask.h>

using namespace mbe;

PixelMask::PixelMask(const sf::Texture & texture) :
	pixelMaskSize(texture.getSize())
{
	this->CreateMaskFromTexture(texture);
}

PixelMask::PixelMask(const sf::Sprite & sprite) :
	pixelMaskSize(sprite.getTexture()->getSize())
{
	this->CreateMaskFromTexture(*sprite.getTexture());
}

PixelMask::PixelMask(const sf::RectangleShape & rectangle)
	: pixelMaskSize(rectangle.getSize())
{
	// Convert the rectange to a texture
	sf::RenderTexture renderTexture;
	renderTexture.create(static_cast<unsigned int>(rectangle.getSize().x), static_cast<unsigned int>(rectangle.getSize().y));
	sf::RectangleShape tempRectangle(rectangle);
	tempRectangle.setPosition(0.0f, 0.0f);
	renderTexture.draw(tempRectangle);
	renderTexture.display();

	CreateMaskFromTexture(renderTexture.getTexture());
}

bool PixelMask::Contains(sf::Vector2f point) const
{
	unsigned int x = static_cast<unsigned int>(std::round(point.x));
	unsigned int y = static_cast<unsigned int>(std::round(point.y));

	// If the point lies within the pixel mask
	if (x >= 0 && x < pixelMaskSize.x && y >= 0 && y < pixelMaskSize.y)
		// Get the pixel using the formula: y * width + x
		return (pixelMask[y * pixelMaskSize.x + x]);

	return false;
}

bool PixelMask::Contains(sf::Vector2f point, const sf::IntRect & subRect) const
{
	int x = static_cast<unsigned int>(std::round(point.x));
	int y = static_cast<unsigned int>(std::round(point.y));

	// Check whether the subrect lies fully within the pixel mask
	if (subRect.left + subRect.width > (int)pixelMaskSize.x || subRect.top + subRect.height > (int)pixelMaskSize.y)
		throw std::runtime_error("PixelMask: The subrect does not lie fully within the pixel mask");

	// Check whether the point lies in the subrect
	if (x < 0 || x > subRect.width || y < 0 || y > subRect.height)
		return false;
	else
		//return pixelMask[x + subRect.left][y + subRect.top] using the formula: y * width + x
		return pixelMask[(y + subRect.top) * pixelMaskSize.x + (x + subRect.left)];
}

void PixelMask::CreateMaskFromTexture(const sf::Texture & texture)
{
	// Create an Image from the given texture
	sf::Image image(texture.copyToImage());

	// measure the time this function takes
	sf::Clock clock;
	sf::Time time = sf::Time::Zero;
	clock.restart();

	// Allocate memory for the pixel mask
	pixelMask = std::make_unique<bool[]>(pixelMaskSize.x * pixelMaskSize.y);

	// Loop through every pixel of the texture
	for (unsigned int y = 0; y < pixelMaskSize.y; y++)
	{
		for (unsigned int x = 0; x < pixelMaskSize.x; x++)
		{
			// If the pixel is not transparrent
			if (image.getPixel(x, y).a > 0)
				// Some part of the texture is there --> insert true
				pixelMask[y * pixelMaskSize.x + x] = true;
			else
				// The user can't see this part of the texture --> insert false
				pixelMask[y * pixelMaskSize.x + x] = false;
		}
	}

	time = clock.restart();
	std::cout << std::endl << "The creation of the pixel mask took: " << time.asMicroseconds() << " microseconds (" << time.asSeconds() << ")";
}
