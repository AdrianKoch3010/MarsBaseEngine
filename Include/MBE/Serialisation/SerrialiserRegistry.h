#pragma once

/// @file
/// @brief Template class SerialiserRegistry

#include <string>
#include <unordered_map>
#include <memory>
#include <exception>
#include <iostream>

#include <MBE/Core/Singleton.h>

namespace mbe
{

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID = TBaseSerialiser::TypeID>
	class SerialiserRegistry : public detail::Singleton<SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>>
	{
	public:
		friend class detail::Singleton<SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>>;

	public:
		typedef std::shared_ptr<SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>> Ptr;
		typedef std::weak_ptr<SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>> WPtr;
		typedef std::unique_ptr<SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>> UPtr;

	private:
		typedef std::unordered_map<std::string, typename TBaseSerialiser::Ptr> SerialiserDictionary;
		typedef std::unordered_map<TSerialiserTypeID, typename TBaseSerialiser::WPtr> SerialserTypeDictionary;
		typedef std::unordered_map<TObjectTypeID, std::string> ObjectTypeDictionary; 
		typedef std::unordered_map<std::string, TObjectTypeID> ObjectNameDictionary;

	public:
		SerialiserRegistry() = default; // Where should the default serialisers be registered?
		~SerialiserRegistry() = default;

	public:
		/// @brief Register a serialiser for an object
		/// @details The serialiser is added for an object of a particular type. This serialiser will then be used
		/// to load / add and store an object of that type from an to an XML element.
		/// @tparam TSerialiser The type of the serialiser to create and add
		/// @tparam TObject The type of the object for which the serialiser's store and load function will be applied
		/// @tparam TArguments The type of the arguments passed
		/// @param objectName A string id for the type of the object. This will be the same as the type attribute in the object's xml node
		/// @param arguments The arguments passed to the constructor of the TSerialiser
		/// @note The serialiser must inherit from TBaseSerialiser
		template<class TSerialiser, class TObject, typename... TArguments>
		void AddSerialiser(const std::string& objectName, TArguments... arguments);

		template<class TObject>
		bool HasSerialiser() const;
		bool HasSerialiser(TObjectTypeID objectTypeId) const;
		bool HasSerialiser(const std::string& objectName) const;

		template<class TSerialiser>
		const TSerialiser& GetSerialiser() const;
		const TBaseSerialiser& GetSerialiser(TObjectTypeID objectTypeId) const;
		const TBaseSerialiser& GetSerialiser(const std::string& objectName) const;

		inline const TBaseSerialiser& operator[](TObjectTypeID objectTypeId) const { return GetSerialiser(objectTypeId); }
		inline const TBaseSerialiser& operator[](const std::string& objectName) const { return GetSerialiser(objectName); }

		const std::string& GetObjectName(TObjectTypeID objectTypeId) const;
		TObjectTypeID GetObjectTypeByName(const std::string& objectName) const;

	private:
		// Maps the object name to the serialiser pointer
		SerialiserDictionary serialiserDictionary;
		// Maps the serialiser type to the serialiser pointer
		SerialserTypeDictionary serialiserTypeDictionary;
		// Maps the object type to the object name
		ObjectTypeDictionary objectTypeDictionary;
		// Maps the object name to the object type
		ObjectNameDictionary objectNameDictionary;
	};

#pragma region TemplateImplementations

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	template<class TSerialiser, class TObject, typename ...TArguments>
	inline void SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::AddSerialiser(const std::string& objectName, TArguments ...arguments)
	{
		std::cerr << "Registered serialiser for " << objectName << std::endl;

		// make sure that TSerialiser inherits from TBaseSerialiser
		static_assert(std::is_base_of<TBaseSerialiser, TSerialiser>::value, "The serialiser must inherit from TBaseSerialiser");

		// Throw if a serialiser for this type already exists
		if (serialiserDictionary.find(objectName) != serialiserDictionary.cend())
			throw std::runtime_error("Serialiser registry: A serialser already exists for this object (" + objectName+ ")");

		// Remember the typeId for this object type for the serialiser store function
		objectTypeDictionary.insert({ TObject::GetTypeID<TObject>(), objectName });

		// Make a new serialser
		auto serialiserPtr = std::make_shared<TSerialiser>(std::forward<TArguments>(arguments)...);

		// Remember it under this type so that it can be accessed later
		serialiserTypeDictionary.insert({ TSerialiser::GetTypeID<TSerialiser>(), serialiserPtr });

		// Add the serialiser to the dictionary
		serialiserDictionary.insert({ objectName, std::move(serialiserPtr) });
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	template<class TObject>
	inline bool SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::HasSerialiser() const
	{
		return objectTypeDictionary.count(TObject::GetTypeID<TObject>()) != 0;
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	template<class TSerialiser>
	inline const TSerialiser& SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::GetSerialiser() const
	{
		const auto typeId = TSerialiser::GetTypeID<TSerialiser>();
		if (serialiserTypeDictionary.find(typeID) == serialiserTypeDictionary.cend())
			throw std::runtime_error("Serialiser registry: No serialser of this type has been registered (" + std::to_string(typeId) + ")");

		auto serialiserPtr = serialiserTypeDictionary.at(typeId).lock();
		return *std::dynamic_pointer_cast<TComponentSerialiser>(serialiserPtr);
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline bool SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::HasSerialiser(TObjectTypeID objectTypeId) const
	{
		return objectTypeDictionary.count(objectTypeId) != 0;
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline bool SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::HasSerialiser(const std::string& objectName) const
	{
		return serialiserDictionary.count(objectName) != 0;
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline const TBaseSerialiser& SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::GetSerialiser(TObjectTypeID objectTypeId) const
	{
		if (objectTypeDictionary.find(objectTypeId) == objectTypeDictionary.cend())
			throw std::runtime_error("Serialiser registry: No serialser of this type has been registered (" + std::to_string(objectTypeId) + ")");

		return *serialiserDictionary.at(objectTypeDictionary.at(objectTypeId));
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline const TBaseSerialiser& SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::GetSerialiser(const std::string& objectName) const
	{
		if (serialiserDictionary.find(objectName) == serialiserDictionary.cend())
			throw std::runtime_error("Serialiser registry: No serialser of this type has been registered (" + objectName + ")");

		return *serialiserDictionary.at(objectName);
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline const std::string& SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::GetObjectName(TObjectTypeID objectTypeId) const
	{
		if (objectTypeDictionary.find(objectTypeId) == objectTypeDictionary.cend())
			throw std::runtime_error("Serialiser registry: No serialser of this type has been registered (" + std::to_string(objectTypeId) + ")");

		return objectTypeDictionary.at(objectTypeId);
	}

	template<class TBaseSerialiser, typename TObjectTypeID, typename TSerialiserTypeID>
	inline TObjectTypeID SerialiserRegistry<TBaseSerialiser, TObjectTypeID, TSerialiserTypeID>::GetObjectTypeByName(const std::string& objectName) const
	{
		if (objectNameDictionary.find(objectName) == objectNameDictionary.cend())
			throw std::runtime_error("Serialiser registry: No serialser of this type has been registered (" + objectName + ")");

		return objectNameDictionary.at(objectName);
	}

#pragma endregion

} // namespace mbe

// Predeclare some serialiser registries
#include <MBE/Serialisation/ComponentSerialiser.h>
#include <MBE/Serialisation/AnimationSerialiser.h>
#include <MBE/Serialisation/AITaskSerialser.h>

namespace mbe
{

	using ComponentSerialiserRegistry = SerialiserRegistry<ComponentSerialiser, Component::TypeID>;
	using AnimationSerialiserRegistry = SerialiserRegistry<AnimationSerialiser, EntityAnimator::AnimationTypeID>;
	using AITaskSerialiserRegistry = SerialiserRegistry<AITaskSerialiser, AITask::TypeID>;

} // namespace mbe

// Define the respective registration macros
#define MBE_REGISTER_COMPONENT_SERIALISER(TSerialiser, TComponent) mbe::ComponentSerialiserRegistry::Instance().AddSerialiser<TSerialiser, TComponent>(MBE_NAME_OF(TComponent));
#define MBE_REGISTER_ANIMATION_SERIALISER(TSerialiser, TAnimation) mbe::AnimationSerialiserRegistry::Instance().AddSerialiser<TSerialiser, TAnimation>(MBE_NAME_OF(TAnimation));
#define MBE_REGISTER_AITASK_SERIALISER(TSerialiser, TAITask) mbe::AITaskSerialiserRegistry::Instance().AddSerialiser<TSerialiser, TAITask>(MBE_NAME_OF(TAITask));
