#pragma once

/// @file
/// @brief mbe::Exception classes

#include <string>
#include <stdexcept>

#include <MBE/Core/Entity.h>

// Types of mbe exceptions
// - Exception (base class for all mbe exceptions)

// The exceptions are divided into fatal exceptions (exceptions that cannot easily be recoved from)
// and NonFatalExceptions (exceptions where the excecution can continue).
// A fatal exception handler will likely break out of the current game state or even the entire game
// whereas a non-fatal exception can be handled close to where it is thrown.
// - FatalException
// - NonFatalException

// Fatal exceptions

// - IDNotFoundException
// - MissingComponentException - needed? or should this be an assert --> Stop using HasComponent everywhere and let the getComponent function throw

// Non fatal exceptions
// - SerialiserException
// - LoadException

namespace mbe
{

	class Exception : public std::runtime_error
	{
	public:
		Exception(const std::string& message);
		Exception(std::string&& message);

	public:
		friend std::ostream& operator<<(std::ostream& os, const Exception& exception);
	};

	class FatalException : public Exception
	{
	public:
		FatalException(const std::string& message) : Exception(message) {}
		FatalException(std::string&& message) : Exception(message) {}
	};

	class NonFatalException : public Exception
	{
	public:
		NonFatalException(const std::string& message) : Exception(message) {}
		NonFatalException(std::string&& message) : Exception(message) {}
	};

	class MissingComponentException : public FatalException
	{
	public:
		MissingComponentException(Entity::HandleID entityId, Component::TypeID componentTypeId);

	public:
		inline Entity::HandleID GetEntityID() const { return entityId; }
		inline Component::TypeID GetComponentTypeID() const { return componentTypeId; }

	private:
		Entity::HandleID entityId;
		Component::TypeID componentTypeId;
	};

	class SerialiserException : public NonFatalException
	{
	public:
		// Creates an error message in the form: {serialiser}Serialiser: Failed to parse {object}
		SerialiserException(const std::string& serialiser, const std::string& object);

	public:
		// The serialiser that was responsible for throwing the exception
		inline const std::string& GetSerialiser() const { return serialiser; }

		// The object that could not be serialised
		inline const std::string& GetObject() const { return object; }

	private:
		std::string serialiser;
		std::string object;
	};

	//// Creates message in the form: IDNotFoundException: no {objectType} id was found 
	//class IDNotFoundException : Exception
	//{
	//public:
	//	IDNotFoundException(const std::string& objectType, const std::string& id);
	//	IDNotFoundException(std::string&& objectType, std::string&& id);
	//	IDNotFoundException(std::string&& objectType, Entity::HandleID id);
	//};

} // namespace mbe