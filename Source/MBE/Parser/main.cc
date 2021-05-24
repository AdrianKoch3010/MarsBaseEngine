#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <exception>
#include "Scanner.h"

struct Token
{
	Token() : type(Scanner::ERROR), content(""), lineNumber(-1) {}
	Token(Scanner::Tokens type, std::string content, int lineNumber) :
		type(type), content(content), lineNumber(lineNumber) {}
	
	Scanner::Tokens type;
	std::string content;
	int lineNumber;
};

typedef std::variant<
			int,
			unsigned,
			long long,
			unsigned long long,
			float,
			double,
			std::string>
			ValueType;


struct Statement
{
	Statement(std::string id, ValueType value) : id(id), value(value) {}
	
	std::string id;
	ValueType value;
};

class Parser
{
public:
	typedef std::vector<Token> TokenList;
	typedef std::vector<Statement> StatementList;
	
	Parser(TokenList& tokenList) : tokenList(tokenList)
	{
		nextToken = tokenList.begin();
	}
	
public:
	inline const StatementList& GetStatementList() const { return statementList; }

	void Parse()
	{
		while(nextToken != tokenList.end())
			ParseStatement();
	}

	void ParseStatement()
	{
		if (nextToken->type == Scanner::IDENTIFIER)
		{
			std::string id = nextToken->content;
			++nextToken;
			if (nextToken->type != Scanner::ASSIGN)
			{
				throw std::runtime_error("Error");
				// error
			}
			++nextToken;
			statementList.emplace_back(Statement(id, ParseValue()));
			++nextToken;
		}
		else
		{
			throw std::runtime_error("Error");
			// error
		}
	}

	ValueType ParseValue()
	{
		if (nextToken->type == Scanner::INT)
		{
			++nextToken;
			return ValueType(42); // Convert the string
		}
		else if (nextToken->type == Scanner::UNSIGNED_INT)
		{
			++nextToken;
			return ValueType(43u); // Convert the string
		}
		else if (nextToken->type == Scanner::LONG)
		{
			++nextToken;
			long long value = 44l; // Convert the string
			return ValueType(value);
		}
		else if (nextToken->type == Scanner::UNSIGNED_LONG)
		{
			++nextToken;
			unsigned long long value = 45ul; // Convert the string
			return ValueType(value);
		}
		else if (nextToken->type == Scanner::DOUBLE)
		{
			++nextToken;
			return ValueType(46.0); // Convert the string
		}
		else if (nextToken->type == Scanner::FLOAT)
		{
			float value = atof(nextToken->content.c_str());
			++nextToken;
			return ValueType(value); // Convert the string
		}
		else if (nextToken->type == Scanner::STRING)
		{
			auto value = nextToken->content;
			++nextToken;
			return ValueType(value);
		}
		else
		{
			throw std::runtime_error("Error");
			// error
		}
	}
	
private:
	TokenList& tokenList;
	TokenList::iterator nextToken;
	StatementList statementList;
};

int main()
{	
	const std::string filePath = "test.txt";
	std::ifstream file(filePath);
	
	if (file.is_open())
		std::cerr << "File opened successfully" << std::endl;
	else
		std::cerr << "Failed to open " << filePath << std::endl;
	
	// Create the scanner and pass in the input file stream
	Scanner scanner(file);
	
	// Create a list of tokens
	std::vector<Token> tokenList;
	
	while(auto token = scanner.lex())
	{
		// Store a token list
		tokenList.push_back(Token(Scanner::Tokens(token), scanner.matched(), scanner.lineNr()));
		
		/* const std::string& text = scanner.matched();
		std::cout << "[" << scanner.lineNr() << "] ";
		switch (token)
		{	
			case Scanner::IDENTIFIER:
				std::cout << "Identifier: " << text << std::endl;
				break;
			case Scanner::INT:
				std::cout << "Int: " << text << std::endl;
				break;
			case Scanner::UNSIGNED_INT:
				std::cout << "Unsigned Int: " << text << std::endl;
				break;
			case Scanner::LONG:
				std::cout << "Long: " << text << std::endl;
				break;
			case Scanner::UNSIGNED_LONG:
				std::cout << "Unsigned Long: " << text << std::endl;
				break;
			case Scanner::DOUBLE:
				std::cout << "Double: " << text << std::endl;
				break;
			case Scanner::FLOAT:
				std::cout << "Float: " << text << std::endl;
				break;
			case Scanner::STRING:
				std::cout << "String: " << text << std::endl;
				break;
			case Scanner::COMMENT:
				std::cout << "Comment: " << text << std::endl;
				break;
			case Scanner::ASSIGN:
				std::cout << "Assign" << std::endl;
				break;
			case Scanner::END_ASSIGN:
				std::cout << "End Assign" << std::endl;
				break;
			case Scanner::ERROR:
				std::cout << "Parse error: " << text << std::endl;
				break;
			default:
				std::cout << "Unknown token: " << text << std::endl;
				break;
		} */
	}
	
	std::cerr << "Token list size: " << tokenList.size() << std::endl;
	
	// Report lex errors and remove comments
	bool containsErrors = false;
	for (auto it = tokenList.begin(); it != tokenList.end();)
	{
		if (it->type == Scanner::ERROR)
		{
			std::cerr << "Unknown token: " << it->content << std::endl;
			containsErrors = true;
			it = tokenList.erase(it);
		}
		else if (it->type == Scanner::COMMENT)
			it = tokenList.erase(it);
		else
			++it;
	}
	
	std::cerr << "Token list size: " << tokenList.size() << std::endl;
	
	if (containsErrors)
		return 0;
	
	// TODO: Some checks to see if the token list is empty
	
	Parser parser(tokenList);
	parser.Parse();
	const auto& statementList = parser.GetStatementList();
	
	for (const auto& statement : statementList)
	{
		std::cout << statement.id << " " << std::get<float>(statement.value) << std::endl;
	}

	return 0;
};