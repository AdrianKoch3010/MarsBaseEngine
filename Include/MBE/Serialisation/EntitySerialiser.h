#pragma once

///@file
///@brief class mbe::EntitySerialser

// C++
#include <string>
#include <memory>
#include <unordered_map>

// TinyXML2
#include <MBE/Core/TinyXML.h>

// SFML
#include <SFML/System/NonCopyable.hpp>

// MBE
#include <MBE/Core/Entity.h>
#include <MBE/Core/Component.h>
#include <MBE/Core/EntityManager.h>
#include <MBE/Core/EntityCreatedEvent.h>
#include <MBE/Core/EventManager.h>
#include <MBE/Core/AssetHolder.h>
#include <MBE/Core/FilePathDictionary.h>

namespace mbe
{
	/// @brief Provides functions for loading and storing entities from and to an XML file format
	/// @details Component serialisers can be registered with the mbe::ComponentSerialiserRegistry.
	/// The registered serialisers will then be used to load and store the components they have been registered for.
	/// The XML is formatted in the following way:
	///
	/// @code
	///	<Entities>
	///	<Entity id="int64_t" parentId="int64_t">
	///		<Component type="string">
	///			// Component data
	///		</Component>
	/// </Entity>
	/// </Entities>
	/// @endcode
	/// @note The entity ids will be reassigned after loading. However, the parent / child entity relations will be preserved.
	class EntitySerialiser : private sf::NonCopyable
	{
		// First is old, second is new
		typedef std::unordered_map<Entity::ID, Entity::ID> EntityIDMap;
		// First is new entity, seonds is old parent entity id
		typedef std::unordered_map<Entity::ID, Entity::ID> ParentEntityIDMap;

	public:
		/// @brief Constructor
		/// @param entityManager A reference to the entityManager whoes entity's will be loaded / stored.
		/// Since the entityIds are reassigned, the entity manager must not be empty
		/// @param eventManager A refernce to the mbe::EventManager
		/// @param context The context for the preregistered component serialisers
		EntitySerialiser(EntityManager& entityManager, EventManager& eventManager/*, Context context*/);

		/// @brief Default destructor
		~EntitySerialiser() = default;

	public:
		// Thorws runtime_error when loading fails 
		// The entity ids will be reassigned. However, the mapping of parents and childs will remain.
		// Returns a list of the entities that have been loaded
		std::vector<Entity::ID> LoadEntities(const std::string& filePath);

		// Prints an error message instead of throwing an error
		// In case the parsing fails, an empty list is returned
		std::vector<Entity::ID> TryLoadEntities(const std::string& filePath, const std::string& fileName);

		// Works the same as LoadEntities but loads from a string rather than a file
		// Returns a list of the entities that have been loaded
		// Throws if the string doesn't contain valid xml
		std::vector<Entity::ID> CreateEntitiesFromString(const std::string& xmlString);

		// Throws runtime_error when storing fails
		void StoreEntites(const std::string& filePath);

	private:
		std::vector<Entity::ID> Load(const tinyxml2::XMLDocument& document);

	private:
		EntityManager& entityManager;
		EventManager& eventManager;

		// Maps the old to the new entity ids
		EntityIDMap entityIdMap;
		// Maps the new entity id to the old parent entity id
		ParentEntityIDMap parentEntityIdMap;
	};

} // namespace mbe