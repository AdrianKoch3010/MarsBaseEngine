#include <iostream>

#include <MBE/Core/Exceptions.h>

using namespace mbe;


Exception::Exception(const std::string& message) : std::runtime_error(message)
{
}

Exception::Exception(std::string&& message) : std::runtime_error(message)
{
}

std::ostream& mbe::operator<<(std::ostream& stream, const Exception& exception)
{
	stream << exception.what();
	return stream;
}

ParseError::ParseError(const std::string& parser, const std::string& message, int lineNumber) :
	NonFatalException(parser + ": Failed to parse " + message),
	parser(parser),
	message(message),
	lineNumber(lineNumber)
{
}

MissingComponentException::MissingComponentException(Entity::HandleID entityId, Component::TypeID componentTypeId) :
	FatalException("The entity (id = " + std::to_string(entityId) + ") does not have the requested component (type = " + std::to_string(componentTypeId) + ")"),
	entityId(entityId),
	componentTypeId(componentTypeId)
{
}
