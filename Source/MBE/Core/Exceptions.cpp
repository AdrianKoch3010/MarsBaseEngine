#include <iostream>

#include <MBE/Core/Exceptions.h>

using namespace mbe;

Exception::

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
