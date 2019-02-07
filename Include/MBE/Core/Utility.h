#pragma once

/// @file
//// @brief Contains helper functions

#include <string>
#include <unordered_map>
#include <algorithm>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

/*!
\def MBE_NAME_OF(x)
Returns x as a c-string or rather embeds x in quotation marks

This is useful when a unique typename is required. One option to make sure that
the name is unique is to use the name of the class. This functions capsules this.
The use of MBE_NAME_OF() should always be preferred over typing the classname as a string
since the IDE can help prevent spelling mistakes and provide useful functionality
such as renaming the class whose name is needed in string form.

\attention When using MBE_NAME_OF() for the above described scenario, make sure that in order to
ensure uniqueness always the full type name is used. This includes e.g. namespaces.
*/
#define MBE_NAME_OF(x) #x

namespace mbe
{
	/// @brief Dictionary for string representations of the sf::Mouse::Button enum
	static const std::unordered_map<sf::Mouse::Button, std::string> mouseButtonStringDictionary = {
		{ sf::Mouse::Button::Left, "Left" },
	{ sf::Mouse::Button::Right, "Right" },
	{ sf::Mouse::Button::Middle, "Middle" },
	{ sf::Mouse::Button::XButton1, "XButton1" },
	{ sf::Mouse::Button::XButton2, "XButton2" },
	{ sf::Mouse::Button::ButtonCount, "Error" }
	};

	enum class Direction : unsigned short int
	{
		Up,
		Down,
		Left,
		Right,
		DirectionCount
	};

	static const std::unordered_map<Direction, std::string> directionStringDictionary = {
		{ Direction::Up, "Up" },
	{ Direction::Down, "Down" },
	{ Direction::Left, "Left" },
	{ Direction::Right, "Right" },
	{ Direction::DirectionCount, "Error" }
	};

	static const std::unordered_map<Direction, sf::Vector2i> directionIntVectDictionary = {
		{ Direction::Up, { 0, -1 }},
	{ Direction::Down, { 0, 1 }},
	{ Direction::Left, { -1, 0 }},
	{ Direction::Right, { 1, 0 }}
	};

	static const std::unordered_map<Direction, sf::Vector2f> directionFloatVectDictionary = {
		{ Direction::Up, { 0.f, -1.f }},
	{ Direction::Down, { 0.f, 1.f }},
	{ Direction::Left, { -1.f, 0.f }},
	{ Direction::Right, { 1.f, 0.f }}
	};

	void NormaliseIDString(std::string & id);

	std::string NormaliseIDString(const std::string & id);

	std::string NormaliseIDString(std::string && id);

	unsigned int TwoToOneDVector(unsigned int x, unsigned int y, unsigned int rowLength);

	/// @brief Converts a point in the cartesian coordinate system to the isometric coordinate system
	/// @param cartesian The point to convert
	sf::Vector2f CartesianToIso(sf::Vector2f cartesian);

	/// @brief Converts a point in the isometric coordinate system to the cartesian coordinate system
	/// @param cartesian The point to convert
	sf::Vector2f IsoToCartesian(sf::Vector2f iso);
}