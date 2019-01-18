#include <MBE/Core/Utility.h>

using namespace mbe;

void NormaliseIDString(std::string & id)
{
	std::transform(id.begin(), id.end(), id.begin(), ::tolower);
}

std::string NormaliseIDString(const std::string & id)
{
	std::string lowerCaseId = id;
	std::transform(lowerCaseId.begin(), lowerCaseId.end(), lowerCaseId.begin(), ::tolower);
	return lowerCaseId;
}

unsigned int TwoToOneDVector(unsigned int x, unsigned int y, unsigned int rowLength)
{
	return y * rowLength + x;
}

sf::Vector2f CartesianToIso(sf::Vector2f cartesian)
{
	sf::Vector2f iso;
	iso.x = cartesian.x - cartesian.y;
	iso.y = (cartesian.x + cartesian.y) / 2;
	return iso;
}

sf::Vector2f IsoToCartesian(sf::Vector2f iso)
{
	sf::Vector2f cartesian;
	cartesian.x = (2 * iso.y + iso.x) / 2;
	cartesian.y = (2 * iso.y - iso.x) / 2;
	return cartesian;
}
