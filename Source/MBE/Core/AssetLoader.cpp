#include <MBE/Core/AssetLoader.h>

using namespace mbe;

std::pair<AssetLoader::TokenLineList, std::string> AssetLoader::LoadTokensAndFile(const std::string& filePath, const std::string fileName)
{
	// Try load the file
	std::ifstream registerFiler{ filePath + fileName };
	if (registerFiler.is_open() == false)
		throw std::runtime_error("AssetLoader: failed to open file (" + filePath + fileName + ")");

	// Tokenise
	TokenLineList lineTokens = Tokenise(registerFiler);

	// Get the folder
	std::string folder = FindFolder(lineTokens);

	return { std::move(lineTokens), std::move(folder) };
}

AssetLoader::TokenLineList AssetLoader::Tokenise(std::ifstream& file)
{
	TokenLineList lineTokens;
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

		// Ignore empty lines and comments
		if (!tokens.empty() && tokens.front().front() != '#')
			lineTokens.emplace_back(std::make_pair(std::move(tokens), lineNumber));
	}
	return lineTokens;
}

std::string AssetLoader::FindFolder(TokenLineList& lineTokens)
{
	// Find the __FOLDER__ token
	std::string folder;
	const auto iterator = std::find_if(lineTokens.begin(), lineTokens.end(), [](const auto& tokensPair) {
		if (!tokensPair.first.empty() && tokensPair.first.front() == "__FOLDER__")
			return true;
		else
			return false;
		});

	if (iterator == lineTokens.end() || iterator->first.size() < 3)
		//throw std::runtime_error("AssetLoader load: The __FOLDER__ keyword has not been defined");
		throw ParseError(MBE_NAME_OF(AssetLoader), "Could not find the __FOLDER__ keyword");

	folder = iterator->first.at(2);
	lineTokens.erase(iterator);

	return folder;
}