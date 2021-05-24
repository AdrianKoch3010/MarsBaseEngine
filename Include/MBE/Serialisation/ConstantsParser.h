#pragma once

/// @ file
/// @brief class ConstantParser

#include <vector>
#include <string>
#include <variant>

#include "../Source/MBE/Parser/Scanner.h"

namespace mbe
{

	class ConstantsParser
	{
	public:
		typedef std::variant<
			int,
			unsigned,
			long long,
			unsigned long long,
			float,
			double,
			std::string>
			ValueType;

		struct Token
		{
			Token() : type(Scanner::ERROR), content(""), lineNumber(-1) {}
			Token(Scanner::Tokens type, std::string content, int lineNumber) :
				type(type), content(content), lineNumber(lineNumber) {}

			Scanner::Tokens type;
			std::string content;
			int lineNumber;
		};

		struct Statement
		{
			Statement(std::string id, ValueType value) : id(id), value(value) {}

			std::string id;
			ValueType value;
		};

	public:
		typedef std::vector<Token> TokenList;
		typedef std::vector<Statement> StatementList;

	public:

		ConstantsParser(TokenList& tokenList);

	public:
		inline const StatementList& GetStatementList() const { return statementList; }

		void Parse();

		void ParseStatement();

		ValueType ParseValue();

	private:
		TokenList& tokenList;
		TokenList::iterator nextToken;
		StatementList statementList;
	};

} // namespace mbe