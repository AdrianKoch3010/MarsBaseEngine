#include <exception>
#include <sstream>
#include <vector>

#include <MBE/Map/TileMapComponent.h>

#include <MBE/Serialisation/TileMapComponentSerialiser.h>

using namespace mbe;

void TileMapComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData) const
{
	using namespace tinyxml2;

	sf::Vector2i position;
	std::vector<std::vector<float>> movementSpeedShape;

	auto& tileMapComponent = entity.AddComponent<TileMapComponent>();

	// Get position
	const auto positionElement = componentData.FirstChildElement("Position");
	if (positionElement != nullptr)
	{
		if (positionElement->QueryIntAttribute("x", &position.x) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TileMapComponentSerialiser), "Failed to parse Position x attribute", positionElement->GetLineNum());
		if (positionElement->QueryIntAttribute("y", &position.y) != XML_SUCCESS)
			throw ParseError(MBE_NAME_OF(TileMapComponentSerialiser), "Failed to parse Position y attribute", positionElement->GetLineNum());
		tileMapComponent.SetPosition(position);
	}

	// Get the movement speed shape
	const auto shapeElement = componentData.FirstChildElement("MovementSpeedShape");
	if (shapeElement != nullptr)
	{
		// Get all the rows
		for (auto rowElement = shapeElement->FirstChildElement("Row"); rowElement != nullptr; rowElement = rowElement->NextSiblingElement("Row"))
		{
			auto rowText = rowElement->GetText();
			if (rowText == nullptr)
				throw ParseError(MBE_NAME_OF(TileMapComponentSerialiser), "Failed to parse row text", rowElement->GetLineNum());
			std::string rowString{ rowText };

			try
			{
				movementSpeedShape.push_back(ParseRow(rowString));
			}
			catch (const std::invalid_argument& error)
			{
				throw ParseError(MBE_NAME_OF(TileMapComponentSerialiser), "Row element was not a float", rowElement->GetLineNum());
			}
		}
		tileMapComponent.SetMovementSpeedShape(std::move(movementSpeedShape));

		// Check that the rows are all equally long
		if (movementSpeedShape.size() != 0)
			for (size_t i = 1; i < movementSpeedShape.size(); i++)
				if (movementSpeedShape.at(i).size() != movementSpeedShape.front().size())
					throw mbe::ParseError(MBE_NAME_OF(TileMapComponentSerialiser), "Rows of the MovementSpeedShape element must be the same size", shapeElement->GetLineNum());
	}
}

void TileMapComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) const
{
	// The entity must have a tile map component (this should be the case when this function is called from the EntitySerialiser)
	if (entity.HasComponent<TileMapComponent>() == false)
		throw std::runtime_error("Store tile map component: The entity must have an mbe::TileMapComponent");

	const auto& tileMapComponent = entity.GetComponent<TileMapComponent>();

	// Store position
	auto positionElement = document.NewElement("Position");
	positionElement->SetAttribute("x", tileMapComponent.GetPosition().x);
	positionElement->SetAttribute("y", tileMapComponent.GetPosition().y);
	componentData.InsertEndChild(positionElement);

	// Store the movement speed shape
	const auto& movementSpeedShape = tileMapComponent.GetMovementSpeedShape();
	auto shapeElement = document.NewElement("MovementSpeedShape");
	// Insert each row
	for (const auto& row : movementSpeedShape)
	{
		auto rowElement = document.NewElement("Row");
		std::stringstream ss;
		std::string rowString;
		for (const auto& speed : row)
		{
			ss << speed << ", ";
		}
		// Remove the last comma
		if (ss.str().size() >= 2)
			rowString = ss.str().substr(0, ss.str().size() - 2);

		rowElement->SetText(rowString.c_str());
		shapeElement->InsertEndChild(rowElement);
	}
	componentData.InsertEndChild(shapeElement);
}

// comma sperated list
std::vector<float> TileMapComponentSerialiser::ParseRow(const std::string& rowString) const
{
	std::vector<float> row;

	std::string currentItemString;
	for (size_t i = 0u; i < rowString.size(); i++)
	{
		// Get the substrings between two '\n' or ','
		const char currentChar = rowString[i];
		if (currentChar == '\n' || currentChar == ',')
		{
			if (!currentItemString.empty())
			{
				/// What does this method return when it fails?
				int speed = std::stof(currentItemString);

				row.push_back(speed);
			}

			// Reset the current item string
			currentItemString = "";
		}
		else
		{
			// Ignore white spaces
			if (currentChar == ' ')
				continue;

			// Add the current char to the current item string
			currentItemString += currentChar;
		}
	}
	// Add last item
	if (!currentItemString.empty())
	{
		/// What does this method return when it fails?
		int speed = std::stof(currentItemString);

		row.push_back(speed);
	}


	return row;
}
