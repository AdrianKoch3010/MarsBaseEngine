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

namespace mbe
{
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
	/// - UtilityAIComponent
	/// - AnimationComponent
	/// - PixelMaskClickableComponent
	/// @note The entity ids will be reassigned after loading. However, the parent / child entity relations will be preserved.
	class EntitySerialiser : private sf::NonCopyable
	{
	private:
		typedef std::unordered_map<std::string, std::unique_ptr<ComponentSerialser>> ComponentSerialiserDictionary;
		typedef std::unordered_map<detail::ComponentTypeID, std::string> ComponentTypeDictionary;
		// First is old, second is new
		typedef std::unordered_map<Entity::HandleID, Entity::HandleID> EntityIDMap;
		// First is new entity, seonds is old parent entity id
		typedef std::unordered_map<Entity::HandleID, Entity::HandleID> ParentEntityIDMap;

	public:
		/// @brief Constructor
		/// @param entityManager A reference to the entityManager whoes entity's will be loaded / stored.
		/// Since the entityIds are reassigned, the entity manager must not be empty
		/// @param eventManager A refernce to the mbe::EventManager
		EntitySerialiser(EntityManager& entityManager, EventManager& eventManager);

		/// @brief Default destructor
		~EntitySerialiser() = default;

	public:
		// Thorws runtime_error when loading fails 
		// The entity ids will be reassigned. However, the mapping of parents and childs will remain.
		void LoadEntites(const std::string& filePath);

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
		template<class TComponentSerialser>
		TComponentSerialser& GetComponentSerialser();

		// Throws if no component serialiser of this type has been found
		// Const overload
		template<class TComponentSerialser>
		const TComponentSerialser& GetComponentSerialser() const;

	private:
		EntityManager& entityManager;
		EventManager& eventManager;

		ComponentSerialiserDictionary componentSerialiserDictionary;
		ComponentTypeDictionary componentTypeDictionary;
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

		// Remember the typeId for this component type for the serialiser store function
		componentTypeDictionary.insert({ detail::GetComponentTypeID<TComponent>(), componentType });

		// Throw if a component serialiser for this type already exists
		if (componentSerialiserDictionary.find(componentType) == componentSerialiserDictionary.end())
			throw std::runtime_error("EntitySerialiser: A component serialser already exists for this component type (" + componentType + ")");

		// Make a new component serialser
		auto componentSerialiserPtr = std::make_unique<TComponentSerialiser>(std::forward<TArguments>(arguments)...);
	
		// Add the component serialiser to the dictionary
		componentSerialiserDictionary.insert({ componentType, std::move(componentSerialiserPtr) });
	}

	template<class TComponentSerialser>
	inline TComponentSerialser& EntitySerialiser::GetComponentSerialser()
	{
		if (componentTypeDictionary.find(detail::GetComponentTypeID<TComponentSerialser>()) == componentTypeDictionary.end())
			throw std::runtime_error("Entity serialiser: No component serialser exists for this type");

		return componentTypeDictionary.at(detail::GetComponentTypeID<TComponentSerialser>());
	}

	template<class TComponentSerialser>
	inline const TComponentSerialser& EntitySerialiser::GetComponentSerialser() const
	{
		if (componentTypeDictionary.find(detail::GetComponentTypeID<TComponentSerialser>()) == componentTypeDictionary.end())
			throw std::runtime_error("Entity serialiser: No component serialser exists for this type");

		return componentTypeDictionary.at(detail::GetComponentTypeID<TComponentSerialser>());
	}

#pragma endregion

} // namespace mbe