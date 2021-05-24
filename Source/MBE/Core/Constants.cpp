#include <fstream>
#include <algorithm>
#include <sstream>

#include <MBE/Core/Constants.h>
#include <MBE/Core/Exceptions.h>

using namespace mbe;

Constants::Constants()
{
}

void Constants::Load(const std::string& filePath)
{
	using Token = ConstantsParser::Token;
	using TokenList = ConstantsParser::TokenList;

	// Open the file
	std::ifstream file(filePath);

	if (!file.is_open())
		throw mbe::ParseError(MBE_NAME_OF(Constants), "Failed to open " + filePath);

	// Create the scanner and pass in the input file stream
	Scanner scanner(file);

	// Create a list of tokens
	TokenList tokenList;
	while (auto token = scanner.lex())
		tokenList.push_back(Token(Scanner::Tokens(token), scanner.matched(), scanner.lineNr()));

	// Report lex errors and remove comments
	TokenList errorTokens;
	for (auto it = tokenList.begin(); it != tokenList.end();)
	{
		if (it->type == Scanner::ERROR)
		{
			std::cerr << "Unknown token: " << it->content << std::endl;
			errorTokens.push_back(*it);
			it = tokenList.erase(it);
		}
		else if (it->type == Scanner::COMMENT)
			it = tokenList.erase(it);
		else
			++it;
	}

	// Create the error string
	if (!errorTokens.empty())
	{
		std::stringstream ss;
		ss << "Syntax errors in line(s):";
		for (const auto& error : errorTokens)
			ss << error.lineNumber << ", ";
		auto errorString = ss.str();

		// Remove the last comma
		errorString.erase(std::prev(errorString.end()));
		errorString.erase(std::prev(errorString.end()));

		throw ParseError(MBE_NAME_OF(ConstantsParser), errorString);
	}

	ConstantsParser parser(tokenList);
	parser.Parse();
	const auto& statementList = parser.GetStatementList();

	// Insert the statements with the correct data type
	for (const auto& statement : statementList)
		Add(statement.id, statement.value);
}

void Constants::Add(const std::string& id, ValueType value)
{
	auto normalId = NormaliseIDString(id);

	if (valueDictionary.count(id))
		throw std::runtime_error("Constants: Id already exists: " + normalId);

	valueDictionary.insert(std::make_pair(normalId, value));
}
