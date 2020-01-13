#include <MBE/Graphics/TextureWrapperComponent.h>

using namespace mbe;
using TextureID = TextureWrapperComponent::TextureID;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<mbe::TextureWrapperComponent>;

TextureWrapperComponent::TextureWrapperComponent(EventManager& eventManager, Entity& parentEntity, const std::string& textureWrapperId) :
	Component(eventManager, parentEntity),
	currentTextureId(0)
{
	AddTexture(textureWrapperId);
}

TextureID TextureWrapperComponent::AddTexture(const std::string& textureWrapperId)
{
	auto textureId = GetHighestTextureID() + 1;

	textureDictionary.insert({ textureId, Texture() });
	textureDictionary.at(textureId).textureWrapper = nullptr;

	// Make sure the component value changed event is raised first so that the textue wrapper is assigned
	SetTextureWrapper(textureWrapperId, textureId, true);

	return textureId;
}

void TextureWrapperComponent::SetActiveTexture(TextureID textureId)
{
	if (textureDictionary.find(textureId) == textureDictionary.cend())
		throw std::runtime_error("TextureWrapperComponent: No texture has been added under this id (" + std::to_string(textureId) + ")");

	currentTextureId = textureId;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureId)));
}

void TextureWrapperComponent::SetTextureWrapper(const std::string& textureWrapperId, TextureID textureId, bool resetTextureRect)
{
	auto& texture = GetTexture(textureId);

	if (texture.textureWrapperId == textureWrapperId)
		return;

	// Assign the new texture wrapper
	texture.textureWrapperId = textureWrapperId;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureWrapper)));

	// Recompute the texture rect if required
	if (resetTextureRect)
		texture.textureRect = { {0, 0}, {static_cast<sf::Vector2i>(texture.textureWrapper->GetTexture().getSize())} };
}

void TextureWrapperComponent::SetTextureWrapper(const std::string& textureWrapperId, bool resetTextureRect)
{
	SetTextureWrapper(textureWrapperId, currentTextureId, resetTextureRect);
}

void TextureWrapperComponent::SetTextureRect(const sf::IntRect& textureRect, TextureID textureId)
{
	auto& texture = GetTexture(textureId);

	// If the current texture rect is the same as the new one
	if (textureRect == texture.textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	if (textureRect.left + textureRect.width > texture.textureWrapper->GetTexture().getSize().x
		|| textureRect.top + textureRect.height > texture.textureWrapper->GetTexture().getSize().y)
		throw std::runtime_error("TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	texture.textureRect = textureRect;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

void TextureWrapperComponent::SetTextureRect(const sf::IntRect& textureRect)
{
	SetTextureRect(textureRect, currentTextureId);
}

void TextureWrapperComponent::SetTextureRect(sf::IntRect&& textureRect, TextureID textureId)
{
	auto& texture = GetTexture(textureId);

	// If the current texture rect is the same as the new one
	if (textureRect == texture.textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	if (textureRect.left + textureRect.width > texture.textureWrapper->GetTexture().getSize().x
		|| textureRect.top + textureRect.height > texture.textureWrapper->GetTexture().getSize().y)
		throw std::runtime_error("TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	texture.textureRect = std::move(textureRect);

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

void TextureWrapperComponent::SetTextureRect(sf::IntRect&& textureRect)
{
	SetTextureRect(std::move(textureRect), currentTextureId);
}

const TextureWrapper& TextureWrapperComponent::GetTextureWrapper(TextureID textureId) const
{
	const auto& texture = GetTexture(textureId);

	if (texture.textureWrapper == nullptr)
		throw std::runtime_error("TextureWrapperComponent: No texture wrapper exists");

	return *texture.textureWrapper;
}

const TextureWrapper& TextureWrapperComponent::GetTextureWrapper() const
{
	return GetTextureWrapper(currentTextureId);
}

const std::string& TextureWrapperComponent::GetTextureWrapperID(TextureID textureId) const
{
	const auto& texture = GetTexture(textureId);

	return texture.textureWrapperId;
}

const std::string& TextureWrapperComponent::GetTextureWrapperID() const
{
	return GetTextureWrapperID(currentTextureId);
}

const sf::IntRect& TextureWrapperComponent::GetTextureRect(TextureID textureId) const
{
	const auto& texture = GetTexture(textureId);
	
	return texture.textureRect;
}

const sf::IntRect& TextureWrapperComponent::GetTextureRect() const
{
	return GetTextureRect(currentTextureId);
}

TextureID TextureWrapperComponent::GetActiveTextureId() const
{
	return currentTextureId;
}

size_t TextureWrapperComponent::GetNumberOfTextures() const
{
	return textureDictionary.size();
}

TextureID TextureWrapperComponent::GetHighestTextureID() const
{
	return textureDictionary.size() - 1;
}

void TextureWrapperComponent::SetTextureWrapper(TextureWrapper* textureWrapper, TextureID textureId)
{
	auto& texture = GetTexture(textureId);
	texture.textureWrapper = textureWrapper;
}

const TextureWrapperComponent::TextureDictionary& TextureWrapperComponent::GetTextureDictionary() const
{
	return textureDictionary;
}

TextureWrapperComponent::Texture& TextureWrapperComponent::GetTexture(TextureID textureId)
{
	if (textureDictionary.find(textureId) == textureDictionary.cend())
		throw std::runtime_error("TextureWrapperComponent: No texture has been added under this id (" + std::to_string(textureId) + ")");

	return textureDictionary.at(textureId);
}

const TextureWrapperComponent::Texture& TextureWrapperComponent::GetTexture(TextureID textureId) const
{
	if (textureDictionary.find(textureId) == textureDictionary.cend())
		throw std::runtime_error("TextureWrapperComponent: No texture has been added under this id (" + std::to_string(textureId) + ")");
	
	return textureDictionary.at(textureId);
}


