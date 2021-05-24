#include <MBE/Serialisation/ConstantsParser.h>
#include <MBE/Core/Exceptions.h>
#include <MBE/Core/Utility.h>

using namespace mbe;

ConstantsParser::ConstantsParser(TokenList& tokenList) : tokenList(tokenList)
{
}

void ConstantsParser::Parse()
{
	nextToken = tokenList.begin();
	while (nextToken != tokenList.end())
		ParseStatement();
}

void ConstantsParser::ParseStatement()
{
	if (nextToken->type == Scanner::IDENTIFIER)
	{
		std::string id = nextToken->content;
		++nextToken;
		if (nextToken->type != Scanner::ASSIGN)
			throw ParseError(MBE_NAME_OF(ConstantsParser), "Expected assignment operator (=)", nextToken->lineNumber);
		++nextToken;
		statementList.emplace_back(Statement(id, ParseValue()));
		if (nextToken->type != Scanner::END_ASSIGN)
			throw ParseError(MBE_NAME_OF(ConstantsParser), "Expected an end of assignment (;)", nextToken->lineNumber);
		++nextToken;
	}
	else
		throw ParseError(MBE_NAME_OF(ConstantsParser), "Excpected an identifier", nextToken->lineNumber);

}

ConstantsParser::ValueType ConstantsParser::ParseValue()
{
	if (nextToken->type == Scanner::INT)
	{
		return std::stoi(nextToken++->content);
		//int value = std::stoi(nextToken++->content);
		//++nextToken;
		//return ValueType(42); // Convert the string
	}
	else if (nextToken->type == Scanner::UNSIGNED_INT)
	{
		return static_cast<unsigned int>(std::stoul(nextToken++->content));
		//unsigned int value = std::stoul(nextToken++->content);
		//++nextToken;
		//return ValueType(43u); // Convert the string
	}
	else if (nextToken->type == Scanner::LONG)
	{
		return std::stoll(nextToken++->content);
		//long long value = std::stoll(nextToken++->content);
		//++nextToken;
		//return ValueType(value);
	}
	else if (nextToken->type == Scanner::UNSIGNED_LONG)
	{
		return std::stoull(nextToken++->content);
		//unsigned long long value = std::stoull(nextToken++->content);
		//++nextToken;
		//return ValueType(value);
	}
	else if (nextToken->type == Scanner::DOUBLE)
	{
		return std::stod(nextToken++->content);
		//double value = std::stod(nextToken++->content);
		//++nextToken;
		//return ValueType(46.0); // Convert the string
	}
	else if (nextToken->type == Scanner::FLOAT)
	{
		return std::stof(nextToken++->content);
		//float value = std::stof(nextToken++->content);
		//++nextToken;
		//return ValueType(value); // Convert the string
	}
	else if (nextToken->type == Scanner::STRING)
	{
		return nextToken++->content;
		//std::string value = nextToken++->content;
		//++nextToken;
		//return ValueType(value);
	}
	else
		throw ParseError(MBE_NAME_OF(ConstantsParser), "Value type not supported", nextToken->lineNumber);
}
