#pragma once

/// @file
/// @brief mbe::Exception classes

#include <string>
#include <stdexcept>

#include <MBE/Core/Entity.h>

// Types of mbe exceptions
// - Exception (base class for all mbe exceptions)
// - FatalException (an exception that cannot be recovered from)
// - IDNotFoundException
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

	//class FatalException : public Exception
	//{
	//public:
	//	FatalException(const std::string& message);
	//	FatalException(std::string&& message);
	//};

	//// Creates message in the form: IDNotFoundException: no {objectType} id was found 
	//class IDNotFoundException : Exception
	//{
	//public:
	//	IDNotFoundException(const std::string& objectType, const std::string& id);
	//	IDNotFoundException(std::string&& objectType, std::string&& id);
	//	IDNotFoundException(std::string&& objectType, Entity::HandleID id);
	//};

} // namespace mbe