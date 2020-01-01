#include <MBE/Graphics/TextureWrapperComponent.h>

using namespace mbe;
using TextureWrapperChangedEvent = mbe::event::ComponentValueChangedEvent<mbe::TextureWrapperComponent>;

TextureWrapperComponent::TextureWrapperComponent(EventManager & eventManager, Entity & parentEntity, const std::string& textureWrapperId) :
	Component(eventManager, parentEntity)
{
	// Make sure the component value changed event is raised first so that the textue wrapper is assigned
	SetTextureWrapper(textureWrapperId, true);
}

void TextureWrapperComponent::SetTextureWrapper(const std::string& textureWrapperId, bool resetTextureRect)
{
	if (this->textureWrapperId == textureWrapperId)
		return;

	// Assign the new texture wrapper
	this->textureWrapperId = textureWrapperId;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureWrapper)));

	// Recompute the texture rect if required
	if (resetTextureRect)
		textureRect = { {0, 0}, {static_cast<sf::Vector2i>(GetTextureWrapper().GetTexture().getSize())} };
}

void TextureWrapperComponent::SetTextureRect(const sf::IntRect & textureRect)
{
	// If the current texture rect is the same as the new one
	if (textureRect == this->textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	assert(textureRect.left + textureRect.width <= GetTextureWrapper().GetTexture().getSize().x
		|| textureRect.top + textureRect.height <= GetTextureWrapper().GetTexture().getSize().y
		&& "TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	this->textureRect = textureRect;

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

void TextureWrapperComponent::SetTextureRect(sf::IntRect && textureRect)
{
	// If the current texture rect is the same as the new one
	if (textureRect == this->textureRect)
		return;

	// Make sure that the texture rect is not bigger than the current texture
	assert(textureRect.left + textureRect.width <= GetTextureWrapper().GetTexture().getSize().x
		|| textureRect.top + textureRect.height <= GetTextureWrapper().GetTexture().getSize().y
		&& "TextureWrapperComponent: The texture rect must lie within the current texture");

	// Assign the new texture rect
	this->textureRect = std::move(textureRect);

	eventManager.RaiseEvent(TextureWrapperChangedEvent(*this, MBE_NAME_OF(textureRect)));
}

const TextureWrapper& TextureWrapperComponent::GetTextureWrapper() const
{
	if (textureWrapper == nullptr)
		throw std::runtime_error("TextureWrapperComponent: No texture wrapper exists");

	return *textureWrapper;
}


