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
