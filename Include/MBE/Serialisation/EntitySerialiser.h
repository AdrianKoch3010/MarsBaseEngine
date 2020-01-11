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
#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Core/AssetHolder.h>
#include <MBE/Core/FilePathDictionary.h>

namespace mbe
{
	namespace detail
	{
		/// @brief Defines the type of the component serialiser id
		/// @details This id is generated at runtime for each component serialiser class by calling mbe::detail::GetComponentSerialiserTypeID()
		typedef std::size_t ComponentSerialserTypeID;

		/// @brief Returns a unique ComponentTypeID (for each function call)
		inline ComponentSerialserTypeID GetComponentSerialiserID() noexcept
		{
			// This will only be initialised once
			static ComponentSerialserTypeID lastId = 0;

			// After the first initialisation a new number will be returned for every function call
			return lastId++;
		}

		/// @brief Returns a unique number (of type std::size_t) for each type T
		/// @details Each component serialiser type will have its own unique id.
		/// The id will be the same for every instance of that type
		/// @tparam TComponentSerialiser The type for which the id is generated. It must inherit from mbe::ComponentSerialiser
		template <typename TComponentSerialiser>
		inline ComponentSerialserTypeID GetComponentSerialiserTypeID() noexcept
		{
			// make sure that TComponentSerialiser inherits from ComponentSerialiser
			static_assert(std::is_base_of<ComponentSerialser, TComponentSerialiser>::value, "The component serialiser must inherit from mbe::ComponentSerialiser");

			// There will be only one static variable for each template type
			static ComponentSerialserTypeID typeId = GetComponentSerialiserID();
			return typeId;
		}
	}

	/// @brief Provides functions for loading and storing entities from and to an XML file format
	/// @details Component serialisers can be added using the AddComponentSerialiser() method.
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
	/// The EntitySerialser already has component serialisers for the
	/// - TransformComponent
	/// - AIComponent
	/// - AnimationComponent
	/// - PixelMaskClickableComponent
	/// - TopDownInformationComponent
	/// - TextureWrapperComponent
	/// - TiledRenderComponent
	/// - SpriteRenderComponent
	/// @note The entity ids will be reassigned after loading. However, the parent / child entity relations will be preserved.
	class EntitySerialiser : private sf::NonCopyable
	{
	private:
		typedef std::unordered_map<std::string, ComponentSerialser::Ptr> ComponentSerialiserDictionary;
		typedef std::unordered_map<detail::ComponentTypeID, std::string> ComponentTypeDictionary;
		typedef std::unordered_map<detail::ComponentSerialserTypeID, ComponentSerialser::WPtr> ComponentSerialserTypeDictionary;

		// First is old, second is new
		typedef std::unordered_map<Entity::HandleID, Entity::HandleID> EntityIDMap;
		// First is new entity, seonds is old parent entity id
		typedef std::unordered_map<Entity::HandleID, Entity::HandleID> ParentEntityIDMap;

	public:
		/// @brief The context required by the preregistered component serialisers
		/// @details The references are passed to the
		/// - TextureWrapperComponentSerialser
		/// - SoundComponentSerialiser
		/// - MusicComponentSerialiser
		/// respectively
		/*struct Context
		{
			TextureWrapperHolder<>& textureWrappers;
			SoundBufferHolder<>& soundBuffers;
			FilePathDictionary<>& musicFilePaths;
		};*/

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
		std::vector<Entity::HandleID> LoadEntites(const std::string& filePath);

		// Works the same as LoadEntities but loads from a string rather than a file
		// Returns a list of the entities that have been loaded
		// Throws if the string doesn't contain valid xml
		std::vector<Entity::HandleID> CreateEntitiesFromString(const std::string& xmlString);

		// Throws runtime_error when storing fails
		void StoreEntites(const std::string& filePath);

		/// @brief Add a component serialiser
		/// @details The serialiser is added for a component of a particular type. This serialiser will then be used
		/// to load / add and store a component of that type from an to an XML element.
		/// @tparam TComponentSerialiser The type of component serialiser to create and add
		/// @tparam TComponent The type of component for which the serialiser's store function will be applied
		/// @tparam TArguments The type of the arguments passed
		/// @param componentType A string id for the type of the component. Thisw will be the same as the type attribute in the component xml node
		/// @param arguments The arguments passed to the constructor of the TComponentSerialiser
		/// @note The componen serialiser must inherit from mbe::ComponentSerialiser
		template<class TComponentSerialiser, class TComponent, typename... TArguments>
		void AddComponentSerialiser(const std::string& componentType, TArguments... arguments);

		// Throws if no component serialiser of this type has been found
		template<class TComponentSerialiser>
		TComponentSerialiser& GetComponentSerialser();

		// Throws if no component serialiser of this type has been found
		// Const overload
		template<class TComponentSerialiser>
		const TComponentSerialiser& GetComponentSerialser() const;

	private:
		std::vector<Entity::HandleID> Load(const tinyxml2::XMLDocument& document);

	private:
		EntityManager& entityManager;
		EventManager& eventManager;
		//Context context;

		ComponentSerialiserDictionary componentSerialiserDictionary;
		ComponentTypeDictionary componentTypeDictionary;
		ComponentSerialserTypeDictionary componentSerialiserTypeDictionary;

		// Maps the old to the new entity ids
		EntityIDMap entityIdMap;
		// Maps the new entity id to the old parent entity id
		ParentEntityIDMap parentEntityIdMap;
	};

#pragma region Template Implementations

	template<class TComponentSerialiser, class TComponent, typename ...TArguments>
	inline void EntitySerialiser::AddComponentSerialiser(const std::string& componentType, TArguments ...arguments)
	{
		// make sure that TComponentSerialiser inherits from ComponentSerialiser
		static_assert(std::is_base_of<ComponentSerialser, TComponentSerialiser>::value, "The component serialiser must inherit from mbe::ComponentSerialiser");

		// Throw if a component serialiser for this type already exists
		if (componentSerialiserDictionary.find(componentType) != componentSerialiserDictionary.end())
			throw std::runtime_error("EntitySerialiser: A component serialser already exists for this component type (" + componentType + ")");

		// Remember the typeId for this component type for the serialiser store function
		componentTypeDictionary.insert({ detail::GetComponentTypeID<TComponent>(), componentType });

		// Make a new component serialser
		auto componentSerialiserPtr = std::make_shared<TComponentSerialiser>(std::forward<TArguments>(arguments)...);

		// Remember it under this type so that it can be accessed later
		componentSerialiserTypeDictionary.insert({ detail::GetComponentSerialiserTypeID<TComponentSerialiser>(), componentSerialiserPtr });
	
		// Add the component serialiser to the dictionary
		componentSerialiserDictionary.insert({ componentType, std::move(componentSerialiserPtr) });
	}

	template<class TComponentSerialiser>
	inline TComponentSerialiser& EntitySerialiser::GetComponentSerialser()
	{
		if (componentSerialiserTypeDictionary.find(detail::GetComponentSerialiserTypeID<TComponentSerialiser()) == componentSerialiserTypeDictionary.end())
			throw std::runtime_error("Entity serialiser: No component serialser exists for this component serialiser type");

		auto componentSerialiserPtr = componentSerialiserTypeDictionary.at(detail::GetComponentSerialiserTypeID<TComponentSerialiser>()).lock();
		return std::dynamic_pointer_cast<TComponentSerialiser>(componentSerialiserPtr);
	}

	template<class TComponentSerialiser>
	inline const TComponentSerialiser& EntitySerialiser::GetComponentSerialser() const
	{
		if (componentSerialiserTypeDictionary.find(detail::GetComponentSerialiserTypeID < TComponentSerialiser()) == componentSerialiserTypeDictionary.end())
			throw std::runtime_error("Entity serialiser: No component serialser exists for this component serialiser type");

		auto componentSerialiserPtr = componentSerialiserTypeDictionary.at(detail::GetComponentSerialiserTypeID<TComponentSerialiser>()).lock();
		return std::dynamic_pointer_cast<TComponentSerialiser>(componentSerialiserPtr);
	}

#pragma endregion

} // namespace mbe