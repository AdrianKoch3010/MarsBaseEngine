#pragma once

/// @file
/// @brief mbe::Exception classes

#include <string>
#include <stdexcept>

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
	class Entity;

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

	/*class MissingComponentException : public FatalException
	{
	public:
		MissingComponentException(Entity::HandleID entityId, Component::TypeID componentTypeId);

	public:
		inline Entity::HandleID GetEntityID() const { return entityId; }
		inline Component::TypeID GetComponentTypeID() const { return componentTypeId; }

	private:
		Entity::HandleID entityId;
		Component::TypeID componentTypeId;
	};*/

	class IDNotFoundException : public FatalException
	{
	public:
		IDNotFoundException(unsigned long long id); // Change id type

	public:
		inline unsigned long long GetID() const { return id; }

	private:
		unsigned long long id;
	};

	// This error will likely be caught by whoever called the parse function and output with the file path and line number
	class ParseError : public NonFatalException
	{
	public:
		// Creates an error message in the form: {parser}: {message}
		// Line number -1 indicates that this error is not particular to any line number
		ParseError(const std::string& parser, const std::string& message, int lineNumber = -1);

	public:
		// The parser that was responsible for throwing the exception
		// This could be a serialiser or e.g. the asset loader
		inline const std::string& GetParser() const { return parser; }

		// The error message
		// E.g. Failed to parse object or something must something
		inline const std::string& GetMessage() const { return message; }

		inline int GetLineNumber() const { return lineNumber; }

	private:
		std::string parser;
		std::string message;
		int lineNumber;
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