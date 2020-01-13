#include <vector>

#include <MBE/Core/Utility.h>
#include <MBE/Graphics/TextureWrapperComponent.h>

#include <MBE/Serialisation/TextureWrapperComponentSerialiser.h>

using namespace mbe;

TextureWrapperComponentSerialiser::TextureWrapperComponentSerialiser() :
	ComponentSerialser()
{
}

void TextureWrapperComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	using namespace tinyxml2;
	using TextureID = TextureWrapperComponent::TextureID;

	TextureID activeTextureId;
	std::vector<std::pair<sf::IntRect, std::string>> textureDataList;
	std::vector<bool> setTextureRectList;

	// Get the active texture id
	const auto activeTextureElement = componentData.FirstChildElement("ActiveTexture");
	if (activeTextureElement == nullptr)
		throw std::runtime_error("Load texture wrapper component: Failed to parse ActiveTexture");
	if (activeTextureElement->QueryUnsignedText(&activeTextureId) != XML_SUCCESS)
		throw std::runtime_error("Load texture wrapper component: Failed to parse ActiveTexture text");

	// Get the textures
	const auto texturesElement = componentData.FirstChildElement("Textures");
	if (texturesElement == nullptr)
		throw std::runtime_error("Load texture wrapper component: Failed to parse Textures");

	for (auto textureElement = texturesElement->FirstChildElement("Texture"); textureElement != nullptr; textureElement = textureElement->NextSiblingElement("Texture"))
	{
		sf::IntRect textureRect;
		// Get the texture rect
		const auto textureRectElement = textureElement->FirstChildElement("TextureRect");
		if (textureRectElement == nullptr)
			setTextureRectList.push_back(false);
		else
		{
			textureRect = IntRectSerialiser::Load(*textureRectElement);
			setTextureRectList.push_back(true);
		}

		// Get the texture wrapper
		const auto textureWrapperElement = textureElement->FirstChildElement("TextureWrapper");
		if (textureWrapperElement == nullptr)
			throw std::runtime_error("Load texture wrapper component: Failed to parse TextureWrapper");
		auto textureWrapperText = textureWrapperElement->GetText();
		if (textureWrapperText == nullptr)
			throw std::runtime_error("Load texture wrapper component: Failed to parese TextureWrapper text");
		std::string textureWrapperString{ textureWrapperText };

		// These will be loaded in order so the ids will match when added in the same order
		textureDataList.push_back(std::make_pair(textureRect, textureWrapperString));
	}
	
	// There must be at least one texture present
	if (textureDataList.empty())
		throw std::runtime_error("Load texture wrapper component: There must be at least one texture");

	// Create the component and set the value
	auto& textureWrapperComponent = entity.AddComponent<TextureWrapperComponent>(textureDataList.front().second);
	if (setTextureRectList.front())
		textureWrapperComponent.SetTextureRect(textureDataList.front().first);

	// Add the remaining textures
	for (TextureID textureId = 1; textureId < textureDataList.size(); textureId++)
	{
		const auto& texture = textureDataList.at(textureId);
		auto id = textureWrapperComponent.AddTexture(texture.second);
		if (setTextureRectList.at(textureId))
			textureWrapperComponent.SetTextureRect(texture.first, textureId);
	}

	// Set the active texture
	textureWrapperComponent.SetActiveTexture(activeTextureId);
}

void TextureWrapperComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	// The entity must have a texture wrapper component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<TextureWrapperComponent>() == false)
		throw std::runtime_error("Store texture wrapper component: The entity must have an mbe::TextureWrapperComponent");
	const auto& textureWrapperComponent = entity.GetComponent<TextureWrapperComponent>();

	// Store the activeTexture id
	auto activeTextureElement = document.NewElement("ActiveTexture");
	activeTextureElement->SetText(textureWrapperComponent.GetActiveTextureId());
	componentData.InsertEndChild(activeTextureElement);

	// Store the textures
	auto texturesElement = document.NewElement("Textures");
	componentData.InsertEndChild(texturesElement);

	using TextureID = TextureWrapperComponent::TextureID;
	for (TextureID textureId = 0; textureId <= textureWrapperComponent.GetHighestTextureID(); textureId++)
	{
		// Store the texture (The id is the order of elements)
		auto textureElement = document.NewElement("Texture");
		texturesElement->InsertEndChild(textureElement);

		// Store the texture rect
		auto textureRectElement = document.NewElement("TextureRect");
		IntRectSerialiser::Store(textureWrapperComponent.GetTextureRect(textureId), document, *textureRectElement);
		textureElement->InsertEndChild(textureRectElement);

		// Store the texture wrapper
		auto textureWrapperElement = document.NewElement("TextureWrapper");
		textureWrapperElement->SetText(textureWrapperComponent.GetTextureWrapperID(textureId).c_str());
		textureElement->InsertEndChild(textureWrapperElement);
	}
}
