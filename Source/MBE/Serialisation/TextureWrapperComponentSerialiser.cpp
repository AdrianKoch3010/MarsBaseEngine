#include <MBE/Core/Utility.h>
#include <MBE/Graphics/TextureWrapperComponent.h>

#include <MBE/Serialisation/TextureWrapperComponentSerialiser.h>

using namespace mbe;

TextureWrapperComponentSerialiser::TextureWrapperComponentSerialiser(const TextureWrapperHolder<std::string>& textureWrapperHolder) :
	ComponentSerialser(),
	textureWrapperHolder(textureWrapperHolder)
{
}

void TextureWrapperComponentSerialiser::LoadComponent(Entity& entity, const tinyxml2::XMLElement& componentData)
{
	using namespace tinyxml2;

	// Get the texture rect
	const auto textureRectElement = componentData.FirstChildElement("TextureRect");
	if (textureRectElement == nullptr)
		throw std::runtime_error("Load texture wrapper component: Failed to parse TextureRect");
	auto textureRect = IntRectSerialiser::Load(*textureRectElement);

	// Get the texture wrapper
	const auto textureWrapperElement = componentData.FirstChildElement("TextureWrapper");
	if (textureWrapperElement == nullptr)
		throw std::runtime_error("Load texture wrapper component: Failed to parse TextureWrapper");
	auto textureWrapperText = textureWrapperElement->GetText();
	if (textureWrapperText == nullptr)
		throw std::runtime_error("Load texture wrapper component: Failed to parese TextureWrapper text");
	std::string textureWrapperString{ textureWrapperText };

	// Create the component and set the value
	entity.AddComponent<TextureWrapperComponent>(textureWrapperHolder, textureWrapperString).SetTextureRect(textureRect);
}

void TextureWrapperComponentSerialiser::StoreComponent(const Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData)
{
	// The entity must have a texture wrapper component (this should be the case when this function is called from the EntitySerialiser
	if (entity.HasComponent<TextureWrapperComponent>() == false)
		throw std::runtime_error("Store texture wrapper component: The entity must have an mbe::TextureWrapperComponent");
	const auto& textureWrapperComponent = entity.GetComponent<TextureWrapperComponent>();

	// Store the texture rect
	auto textureRectElement = document.NewElement("TextureRect");
	IntRectSerialiser::Store(textureWrapperComponent.GetTextureRect(), document, *textureRectElement);
	componentData.InsertEndChild(textureRectElement);

	// Store the texture wrapper
	auto textureWrapperElement = document.NewElement("TextureWrapper");
	textureWrapperElement->SetText(textureWrapperComponent.GetTextureWrapperName().c_str());
	componentData.InsertEndChild(textureWrapperElement);
}
