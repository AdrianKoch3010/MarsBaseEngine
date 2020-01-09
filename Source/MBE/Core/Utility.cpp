#include <MBE/Core/Utility.h>


void mbe::NormaliseIDString(std::string & id)
{
	std::transform(id.begin(), id.end(), id.begin(), ::tolower);
}

std::string mbe::NormaliseIDString(const std::string & id)
{
	std::string lowerCaseId = id;
	std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);
	return lowerCaseId;
}

std::string mbe::NormaliseIDString(std::string && id)
{
	std::transform(id.begin(), id.end(), id.begin(), ::tolower);
	return std::move(id);
}

unsigned int mbe::TwoToOneDVector(unsigned int x, unsigned int y, unsigned int rowLength)
{
	return y * rowLength + x;
}

sf::Vector2f mbe::CartesianToIso(sf::Vector2f cartesian)
{
	sf::Vector2f iso;
	iso.x = cartesian.x - cartesian.y;
	iso.y = (cartesian.x + cartesian.y) / 2;
	return iso;
}

sf::Vector2f mbe::IsoToCartesian(sf::Vector2f iso)
{
	sf::Vector2f cartesian;
	cartesian.x = (2 * iso.y + iso.x) / 2;
	cartesian.y = (2 * iso.y - iso.x) / 2;
	return cartesian;
}

sf::IntRect mbe::IntRectSerialiser::Load(const tinyxml2::XMLElement& rectData)
{
	using namespace tinyxml2;

	sf::IntRect rect;

	const auto topElement = rectData.FirstChildElement("Top");
	if (topElement == nullptr)
		throw std::runtime_error("Load int rect: Failed to parse Top element");
	if (topElement->QueryIntText(&rect.top) != XML_SUCCESS)
		throw std::runtime_error("Load int rect: Failed to parse Top text");

	const auto leftElement = rectData.FirstChildElement("Left");
	if (leftElement == nullptr)
		throw std::runtime_error("Load int rect: Failed to parse Left element");
	if (leftElement->QueryIntText(&rect.left) != XML_SUCCESS)
		throw std::runtime_error("Load int rect: Failed to parse Left text");

	const auto widthElement = rectData.FirstChildElement("Width");
	if (widthElement == nullptr)
		throw std::runtime_error("Load int rect: Failed to parse Width element");
	if (widthElement->QueryIntText(&rect.width) != XML_SUCCESS)
		throw std::runtime_error("Load int rect: Failed to parse Width text");

	const auto heightElement = rectData.FirstChildElement("Height");
	if (heightElement == nullptr)
		throw std::runtime_error("Load int rect: Failed to parse Height element");
	if (heightElement->QueryIntText(&rect.height) != XML_SUCCESS)
		throw std::runtime_error("Load int rect: Failed to parse Height text");

	return rect;
}

void mbe::IntRectSerialiser::Store(const sf::IntRect& rect, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& rectData)
{
	auto topElement = document.NewElement("Top");
	topElement->SetText(rect.top);
	rectData.InsertEndChild(topElement);

	auto leftElement = document.NewElement("Left");
	leftElement->SetText(rect.left);
	rectData.InsertEndChild(leftElement);

	auto widthElement = document.NewElement("Width");
	widthElement->SetText(rect.width);
	rectData.InsertEndChild(widthElement);

	auto heightElement = document.NewElement("Height");
	heightElement->SetText(rect.height);
	rectData.InsertEndChild(heightElement);
}

sf::Color mbe::ColourSerialiser::Load(const tinyxml2::XMLElement& colourData)
{
	using namespace tinyxml2;

	unsigned int r, g, b, a;

	const auto rElement = colourData.FirstChildElement("R");
	if (rElement == nullptr)
		throw std::runtime_error("Load colour: Failed to parse R element");
	if (rElement->QueryUnsignedText(&r) != XML_SUCCESS)
		throw std::runtime_error("Load int colour: Failed to parse R text");

	const auto gElement = colourData.FirstChildElement("G");
	if (gElement == nullptr)
		throw std::runtime_error("Load int colour: Failed to parse G element");
	if (gElement->QueryUnsignedText(&g) != XML_SUCCESS)
		throw std::runtime_error("Load int colour: Failed to parse G text");

	const auto bElement = colourData.FirstChildElement("B");
	if (bElement == nullptr)
		throw std::runtime_error("Load int colour: Failed to parse B element");
	if (bElement->QueryUnsignedText(&b) != XML_SUCCESS)
		throw std::runtime_error("Load int colour: Failed to parse B text");

	const auto aElement = colourData.FirstChildElement("A");
	if (aElement == nullptr)
		throw std::runtime_error("Load int colour: Failed to parse A element");
	if (aElement->QueryUnsignedText(&a) != XML_SUCCESS)
		throw std::runtime_error("Load int colour: Failed to parse A text");

	return sf::Color{ static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b), static_cast<sf::Uint8>(a) };
}

void mbe::ColourSerialiser::Store(const sf::Color& colour, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& colourData)
{
	auto rEement = document.NewElement("R");
	rEement->SetText(colour.r);
	colourData.InsertEndChild(rEement);

	auto gElement = document.NewElement("G");
	gElement->SetText(colour.g);
	colourData.InsertEndChild(gElement);

	auto bElement = document.NewElement("B");
	bElement->SetText(colour.b);
	colourData.InsertEndChild(bElement);

	auto aElement = document.NewElement("A");
	aElement->SetText(colour.a);
	colourData.InsertEndChild(aElement);
}
