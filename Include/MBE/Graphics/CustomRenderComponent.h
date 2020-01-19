#pragma once

/// @file
/// @brief Class mbe::CustomRenderComponent

#include <memory>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <MBE/Graphics/RenderComponent.h>


namespace mbe
{

	class CustomRenderComponent : public mbe::RenderComponent
	{
	public:
		typedef std::shared_ptr<CustomRenderComponent> Ptr;
		typedef std::weak_ptr<CustomRenderComponent> WPtr;
		typedef std::unique_ptr<CustomRenderComponent> UPtr;

	public:
		CustomRenderComponent(mbe::EventManager& eventManager, mbe::Entity& parentEntity);
		~CustomRenderComponent() = default;

	public:
		virtual void CustomRenderComponent::Draw(sf::RenderTarget& target) const;

		virtual sf::FloatRect CustomRenderComponent::GetLocalBounds() const;

	public:
		void SetLocalBounds(const sf::FloatRect& bounds);

		void SetRenderStates(const sf::RenderStates& states);

		const sf::RenderStates& GetRenderStates() const;

		sf::VertexArray& GetVertices();

		const sf::VertexArray& GetVertices() const;

	protected:
		sf::VertexArray vertices;
		sf::RenderStates renderStates;
		sf::FloatRect localBounds;
	};

}