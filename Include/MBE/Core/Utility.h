#pragma once

/// @file
/// @brief Contains helper functions

#include <string>
#include <unordered_map>
#include <algorithm>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <MBE/Core/TinyXML.h>
#include <MBE\Core\Component.h>

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

	/// @brief Standard method for normalising a string
	/// @details This method is used by most classes within the Mars Base Engine that use string ids such as the mbe::AssetHolder.
	/// Normalisation will make all letters lowercase.
	/// @param id The string to be normalised
	void NormaliseIDString(std::string& id);

	/// @brief Standard method for normalising a string
	/// @details This method is used by most classes within the Mars Base Engine that use string ids such as the mbe::AssetHolder.
	/// Normalisation will make all letters lowercase.
	/// @param id The string to be normalised
	/// @returns The normalised id string
	std::string NormaliseIDString(const std::string& id);

	/// @brief Standard method for normalising a string
	/// @details This method is used by most classes within the Mars Base Engine that use string ids such as the mbe::AssetHolder.
	/// Normalisation will make all letters lowercase.
	/// @param id The string to be normalised
	/// @returns The normalised id string
	std::string NormaliseIDString(std::string&& id);

	/// @brief Function for mapping from 2D to 1D vector
	/// @details Calculates the index in a one dimensional vector that is used to represent two dimensional data.
	/// @param x The x position in the imaginary 2D vector
	/// @param y The y position in the imaginary 2D vector
	/// @param rowLength The length of each row in the imaginary 2D vector
	/// @returns The resulting index in the one dimensional vector
	unsigned int TwoToOneDVector(unsigned int x, unsigned int y, unsigned int rowLength);

	/// @brief Converts a point in the cartesian coordinate system to the isometric coordinate system
	/// @param cartesian The point to convert
	sf::Vector2f CartesianToIso(sf::Vector2f cartesian);

	/// @brief Converts a point in the isometric coordinate system to the cartesian coordinate system
	/// @param cartesian The point to convert
	sf::Vector2f IsoToCartesian(sf::Vector2f iso);

	/// @brief Class providing methods for loading and storing an sf::IntRect
	/// @details XML format
	/// @code
	/// <RectName>
	///		<Top>int</Top>
	///		<Left>int</Left>
	///		<Width>int</Width>
	///		<Height>int</Height>
	/// </RectName>
	/// @endcode
	class IntRectSerialiser
	{
	public:
		/// @brief Default constructor
		IntRectSerialiser() = default;

		/// @brief Default destructor
		~IntRectSerialiser() = default;

	public:
		/// @brief Load an sf::IntRect from an XML element
		/// @param rectData The XML element that is parsed
		/// @returns An sf::IntRect with the loaded values
		/// @throws If the parsing fails
		static sf::IntRect Load(const tinyxml2::XMLElement& rectData);

		/// @brief Store an sf::IntRect to an XML element
		/// @param rect The sf::IntRect to be stored
		/// @param document The XML document to which the rect is stored to
		/// @param rectData The XML element to which the rect data is stored to
		static void Store(const sf::IntRect& rect, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& rectData);
	};

	/// @brief Class providing methods for loading and storing sf::Color
	/// @details XML format
	/// @code
	/// <ColourName>
	///		<R>unsigned int</R>
	///		<G>unsigned int</G>
	///		<B>unsigned int</B>
	///		<A>unsigned int</A>
	/// </ColourName>
	/// @endcode
	class ColourSerialiser
	{
	public:
		/// @brief Default constructor
		ColourSerialiser() = default;

		/// @brief Default destructor
		~ColourSerialiser() = default;

	public:
		/// @brief Load an sf::IntRect from an XML element
		/// @param colourData The XML element that is parsed
		/// @returns The sf::Color with the loaded values
		/// @throws If the parsing fails
		static sf::Color Load(const tinyxml2::XMLElement& colourData);

		/// @brief Store an sf::Color to an XML element
		/// @param colour The sf::Color to be stored
		/// @param document The XML document to which the colour is stored to
		/// @param colourData The XML element to which the colour data is stored to
		static void Store(const sf::Color& colour, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& colourData);
	};

} // namespace