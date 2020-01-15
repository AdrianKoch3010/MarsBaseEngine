#include <MBE/Core/AssetLoader.h>

using namespace mbe;

void AssetLoader::Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName)
{
	// Try load the file
	std::ifstream registerFiler{ filePath + fileName };
	if (registerFiler.is_open() == false)
		throw std::runtime_error("AssetLoader failed to open register file (" + fileName + ")");

	std::string line;
	size_t lineNumber = 0;
	std::unordered_map<std::string, std::string> assetDictionary;
	while (std::getline(registerFiler, line))
	{
		lineNumber++;

		// Tokenise
		std::vector<std::string> tokens;
		std::string currentToken;
		for (char currentChar : line)
		{
			if (currentChar == ' ' || currentChar == ':' || currentChar == '=' || currentChar == '\t')
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
		if (tokens.size() < 2)
			invalidSyntax = true;

		// The first two tokens must not contain any tokens
		else if (tokens[0].find('#') != std::string::npos || tokens[1].find('#') != std::string::npos)
			invalidSyntax = true;

		// If there are more than two tokens, the third must start with a comment
		else if (tokens.size() > 2 && tokens[2][0] != '#')
			invalidSyntax = true;

		if (invalidSyntax)
			std::cerr << "AssetLoader: Failed to parse line " << lineNumber << " (" << fileName << ")" << std::endl;
		else
			assetDictionary.insert({ tokens[0], tokens[1] });
	}

	// Attempt to load the assets
	// Check if the __FOLDER__ has been defined
	std::string folder;
	if (assetDictionary.find("__FOLDER__") == assetDictionary.cend())
		throw std::runtime_error("AssetLoader load: The __FOLDER__ keyword has not been defined (" + fileName + ")");
	folder = assetDictionary.at("__FOLDER__");
	assetDictionary.erase("__FOLDER__");

	for (const auto& pair : assetDictionary)
	{
		auto assetId = pair.first;
		const auto& file = pair.second;
		bool loadPixelMask = false;

		// If the texture id ends on a *, remove it and load the pixel mask
		if (assetId[assetId.size() - 1] == '*')
		{
			assetId = assetId.substr(0, assetId.size() - 1);
			loadPixelMask = true;
		}

		// Make sure the id wasn't just a dot
		if (assetId.empty())
		{
			std::cerr << "AssetLoader load textures: * isn't a valid texture Id " << "(" << fileName << ")" << std::endl;
			continue; // Make sure not to load this asset
		}

		if (loadPixelMask)
			assetHolder.Load(assetId, filePath + folder + file, true);
		else
			assetHolder.Load(assetId, filePath + folder + file, false);
	}
}

void AssetLoader::Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName)
{
	// Try load the file
	std::ifstream registerFiler{ filePath + fileName };
	if (registerFiler.is_open() == false)
		throw std::runtime_error("AssetLoader failed to open register file (" + fileName + ")");

	std::string line;
	size_t lineNumber = 0;
	std::unordered_map<std::string, std::string> assetDictionary;
	while (std::getline(registerFiler, line))
	{
		lineNumber++;

		// Tokenise
		std::vector<std::string> tokens;
		std::string currentToken;
		for (char currentChar : line)
		{
			if (currentChar == ' ' || currentChar == ':' || currentChar == '=' || currentChar == '\t')
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
		if (tokens.size() < 2)
			invalidSyntax = true;

		// The first two tokens must not contain any tokens
		else if (tokens[0].find('#') != std::string::npos || tokens[1].find('#') != std::string::npos)
			invalidSyntax = true;

		// If there are more than two tokens, the third must start with a comment
		else if (tokens.size() > 2 && tokens[2][0] != '#')
			invalidSyntax = true;

		if (invalidSyntax)
			std::cerr << "AssetLoader: Failed to parse line " << lineNumber << " (" << fileName << ")" << std::endl;
		else
			assetDictionary.insert({ tokens[0], tokens[1] });
	}

	// Attempt to load the assets
	// Check if the __FOLDER__ has been defined
	std::string folder;
	if (assetDictionary.find("__FOLDER__") == assetDictionary.cend())
		throw std::runtime_error("AssetLoader load: The __FOLDER__ keyword has not been defined (" + fileName + ")");
	folder = assetDictionary.at("__FOLDER__");
	assetDictionary.erase("__FOLDER__");

	for (const auto& pair : assetDictionary)
	{
		auto id = pair.first;
		const auto& file = pair.second;

		filePathDictionary.Insert(id, file);
	}
}
