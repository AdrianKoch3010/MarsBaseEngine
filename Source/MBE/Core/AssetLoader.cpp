#include <MBE/Core/AssetLoader.h>

using namespace mbe;

void AssetLoader::Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName)
{
	// Try load the file
	std::ifstream registerFiler{ filePath + fileName };
	if (registerFiler.is_open() == false)
		throw std::runtime_error("AssetLoader failed to open register file (" + fileName + ")");

	// Tokenise
	std::vector<std::vector<std::string>> lineTokens;
	try
	{
		lineTokens = Tokenise(registerFiler);
	}
	catch (const std::runtime_error& error)
	{
		throw std::runtime_error(std::string(error.what()) + " (" + fileName + ")");
	}

	// Get the folder
	std::string folder = "";
	try
	{
		folder = FindFolder(lineTokens);
	}
	catch (const std::runtime_error & error)
	{
		throw std::runtime_error(std::string(error.what()) + " (" + fileName + ")");
	}

	std::string currentSubFolder = "";
	for (const auto& tokens : lineTokens)
	{
		// If this is a subfolder declaration
		if (tokens.size() == 1 && tokens.front().front() == '[')
		{
			currentSubFolder = tokens.front();
			// Remove the brackets
			currentSubFolder.erase(0, 1);
			currentSubFolder.erase(currentSubFolder.size() - 1, 1);
		}
		// If this is an asset declaration
		else if (tokens.size() >= 3)
		{
			const auto& assetId = tokens.at(0);
			const auto& op = tokens.at(1);
			const auto& fileString = tokens.at(2);

			// Switch on the operator
			if (op == "=")
				assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString, false);
			else if (op == "*=")
				assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString, true);
			else
				std::cerr << "AssetLoader load textures: invalid operator" << "(" << fileName << ")" << std::endl;
		}
		else
			std::cerr << "AssetLoader load textures: invalid declaration" << "(" << fileName << ")" << std::endl;
	}
}

void AssetLoader::Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName)
{
	// Try load the file
	std::ifstream registerFiler{ filePath + fileName };
	if (registerFiler.is_open() == false)
		throw std::runtime_error("AssetLoader failed to open register file (" + fileName + ")");

	// Tokenise
	std::vector<std::vector<std::string>> lineTokens;
	try
	{
		lineTokens = Tokenise(registerFiler);
	}
	catch (const std::runtime_error & error)
	{
		throw std::runtime_error(std::string(error.what()) + " (" + fileName + ")");
	}

	// Get the folder
	std::string folder = "";
	try
	{
		folder = FindFolder(lineTokens);
	}
	catch (const std::runtime_error& error)
	{
		throw std::runtime_error(std::string(error.what()) + " (" + fileName + ")");
	}

	std::string currentSubFolder = "";
	for (const auto& tokens : lineTokens)
	{
		// If this is a subfolder declaration
		if (tokens.size() == 1 && tokens.front().front() == '[')
		{
			currentSubFolder = tokens.front();
			// Remove the brackets
			currentSubFolder.erase(0, 1);
			currentSubFolder.erase(currentSubFolder.size() - 1, 1);
		}
		// If this is an asset declaration
		else if (tokens.size() >= 3)
		{
			const auto& id = tokens.at(0);
			const auto& op = tokens.at(1);
			const auto& fileString = tokens.at(2);

			// Switch on the operator
			if (op == "=")
				filePathDictionary.Insert(id, filePath + folder + currentSubFolder + fileString);
			else
				std::cerr << "AssetLoader load file path dictionary: invalid operator" << "(" << fileName << ")" << std::endl;
		}
		else
			std::cerr << "AssetLoader load file path dictionary: invalid declaration" << "(" << fileName << ")" << std::endl;
	}
}

std::vector<std::vector<std::string>> AssetLoader::Tokenise(std::ifstream& file)
{
	std::vector<std::vector<std::string>> lineTokens;
	std::string line;
	size_t lineNumber = 0;
	while (std::getline(file, line))
	{
		lineNumber++;

		// Tokenise
		std::vector<std::string> tokens;
		std::string currentToken;
		for (char currentChar : line)
		{
			// If it's a seperator
			if (currentChar == ' ' || currentChar == '\t')
			{
				if (!currentToken.empty())
				{
					tokens.push_back(currentToken);
					currentToken.clear();
				}
			}
			else
				currentToken.push_back(currentChar);
		}
		if (!currentToken.empty())
			tokens.push_back(currentToken);

		// If the line begins with a comment
		if (tokens.empty() || tokens.front()[0] == '#')
			continue;

		bool invalidSyntax = false;
		if (tokens.size() < 3)
		{
			// Could be a subfolder declaration if there is only one token and it starts with '[' and ends with ']'
			if ((tokens.size() == 1
				&& tokens.front().front() == '['
				&& tokens.front().at(tokens.front().size() - 1) == ']')
				== false)
				invalidSyntax = true;
		}

		// The first 3 tokens must not contain any commnets
		else if (tokens[0].find('#') != std::string::npos
			|| tokens[1].find('#') != std::string::npos
			|| tokens[2].find('#') != std::string::npos)
			invalidSyntax = true;

		// If there are more than 3 tokens, the 4th must start with a comment
		else if (tokens.size() > 3 && tokens[2][0] != '#')
			invalidSyntax = true;

		if (invalidSyntax)
			throw std::runtime_error("AssetLoader: Failed to parse line " + std::to_string(lineNumber) + ": Too many line arguments");
		else
			lineTokens.emplace_back(std::move(tokens));
	}

	return lineTokens;
}

std::string AssetLoader::FindFolder(std::vector<std::vector<std::string>>& lineTokens)
{
	// Find the __FOLDER__ token
	std::string folder;
	const auto iterator = std::find_if(lineTokens.begin(), lineTokens.end(), [](const auto& tokens) {
		if (tokens.front() == "__FOLDER__")
			return true;
		else
			return false;
		});
	if (iterator == lineTokens.end() || iterator->size() < 3)
		throw std::runtime_error("AssetLoader load: The __FOLDER__ keyword has not been defined");

	folder = iterator->at(2);
	lineTokens.erase(iterator);

	return folder;
}