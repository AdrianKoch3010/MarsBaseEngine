#pragma once

/// @file
/// @brief class mbe::AssetLoader


#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <iostream>

#include <MBE/Core/AssetHolder.h>
#include <MBE/Core/FilePathDictionary.h>

namespace mbe
{

	class AssetLoader
	{
	public:
		template <class TAssetHolder>
		static void Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::TextureWrapperHolder
		static void Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::MusicDictionary
		static void Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName);
	};

} // namespace mbe


#pragma region Template Implementations

template<class TAssetHolder>
inline void mbe::AssetLoader::Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName)
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
		const auto& assetId = pair.first;
		const auto& file = pair.second;
		assetHolder.Load(assetId, filePath + folder + file);
	}
}

#pragma endregion
