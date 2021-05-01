#pragma once

#include <string>
#include <memory>
#include <vector>

#include <MBE/Core/TinyXML.h>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityManager.h>

#include <MBE/TransformComponent.h>
#include <MBE/Graphics/TextureWrapperComponent.h>
#include <MBE/Graphics/RenderInformationComponent.h>
#include <MBE/Graphics/TiledRenderComponent.h>
#include <MBE/Map/TileComponent.h>


namespace mbe
{
	// The entity that is used for the layer only represents the render part
	class TiledTerrain
	{
	public:
		//typedef TiledTerrainLayer::ID LayerID;
		typedef std::unique_ptr<TiledTerrain> UPtr;

#pragma region Local Class Data
	public:
		class Data
		{
		public:
			typedef std::shared_ptr<Data> Ptr;

		public:
			// Throughs exception
			void Load(const std::string& filePath);

			// Automatically converts the std::vector<int> to a c-style int array (int *)
			inline const std::vector<std::vector<size_t>>& GetTileMapLayersIndexList() const { return tileMapLayersIndexList; }
			inline sf::Vector2u GetTileSize() const { return tileSize; }
			inline sf::Vector2u GetMapSize() const { return mapSize; }

		private:
			/// Later void and acces class data (or static)
			// The passed in string must begin and end with either a '\n' or ',' and all
			// data items must be seperated by one of these characters
			void ParseTileIndices(const std::string& tileMapLayerData);

			std::vector<std::vector<size_t>> tileMapLayersIndexList;
			sf::Vector2u tileSize{ 0u, 0u };
			sf::Vector2u mapSize{ 0u, 0u };
		};
#pragma endregion

	public:
		TiledTerrain(EventManager& eventManager, EntityManager& entityManager, sf::Vector2u size, sf::Vector2u tileSize);
		TiledTerrain(EventManager& eventManager, EntityManager& entityManager, Data::Ptr mapData, const std::string& tileMapTextureWrapperId);

		// Unsubscribe from event
		~TiledTerrain();

	public:

		inline const sf::Vector2u& GetSize() const { return size; }
		inline const sf::Vector2u& GetTileSize() const { return tileSize; }

		// The zOrder Of the layer is automatically set to last
		Entity::ID AddTileMapLayer(const std::string& textureWrapperId);
		Entity::ID GetLayer(const size_t layerIndex);
		void SwopRenderLayerOrder(const size_t first, const size_t second);

		inline Entity::ID operator[](const size_t layerIndex) { return GetLayer(layerIndex); }

	private:
		void RecalculateLayer(Entity& entity);

		void SubscribeEvents();
		void OnTextureWrapperChangedEvent(TextureWrapperComponent& textureWraapperComponent);
		void OnIndexListChangedEvent(TileComponent& tileComponent);

	private:
		const sf::Vector2u size;
		const sf::Vector2u tileSize;

		std::vector<Entity::ID> renderLayerList;
		std::vector<std::vector<size_t>> tileMapLayersIndexList;

		EntityManager& entityManager;
		EventManager& eventManager;
		EventManager::SubscriptionID componentChangedSubscription;
	};

} // namespace mbe

