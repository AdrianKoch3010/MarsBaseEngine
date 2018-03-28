#pragma once

#include <string>
#include <memory>
#include <vector>

#include <MBE/Core/TinyXML.h>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EntityManager.h>
#include <MBE/TiledTerrainLayer.h>

#include <MBE/TransformComponent.h>
#include <MBE/TiledTerrainLayerComponent.h>
#include <MBE/Graphics/TiledTerrainLayerRenderComponent.h>
#include <MBE/TiledTerrainLayerSystem.h>

namespace mbe
{
	class TiledTerrain
	{
	public:
		//typedef TiledTerrainLayer::HandleID LayerID;
		typedef std::unique_ptr<TiledTerrain> UPtr;

#pragma region Local Class Data
	public:
		class Data
		{
		public:
			typedef std::shared_ptr<Data> Ptr;

		public:
			// Throughs exception
			void Load(const std::string filePath);

			// Automatically converts the std::vector<int> to a c-style int array (int *)
			inline const std::vector<std::vector<size_t>> & GetTileMapLayersIndexList() const { return tileMapLayersIndexList; }
			inline sf::Vector2u GetTileSize() const { return tileSize; }
			inline sf::Vector2u GetMapSize() const { return mapSize; }

		private:
			/// Later void and acces class data (or static)
			// The passed in string must begin and end with either a '\n' or ',' and all
			// data items must be seperated by one of these characters
			void ParseTileIndices(std::string tileMapLayerData);

			std::vector<std::vector<size_t>> tileMapLayersIndexList;
			sf::Vector2u tileSize{ 0u, 0u };
			sf::Vector2u mapSize{ 0u, 0u };
		};
#pragma endregion
		
	public:
		TiledTerrain(EventManager & eventManager, EntityManager & entityManager, sf::Vector2u size, sf::Vector2u tileSize);
		TiledTerrain(EventManager & eventManager, EntityManager & entityManager, Data::Ptr mapData, const sf::Texture & tileMapTexture);
		~TiledTerrain() = default;

	public:

		inline sf::Vector2u GetSize() const { return size; }
		inline sf::Vector2u GetTileSize() const { return tileSize; }

		// The zOrder Of the layer is automatically set to last
		Entity::HandleID AddTileMapLayer(const sf::Texture & texture);
		Entity::HandleID GetLayer(const size_t layerIndex);
		void SwopRenderLayerOrder(const size_t first, const size_t second);

		inline Entity::HandleID operator[](const size_t layerIndex) { return GetLayer(layerIndex); }


	private:
		const sf::Vector2u size;
		const sf::Vector2u tileSize;

		std::vector<Entity::HandleID> layerList;
		//std::vector<TiledTerrainLayer::UPtr> tileMapLayerList;
		//std::vector<std::unique_ptr<RenderNode>> tileMapLayerRenderNodeList;
		
		EventManager & eventManager;
		EntityManager & entityManager;

		TiledTerrainLayerSystem tiledTerrainLayerSystem;
	};

} // namespace mbe

