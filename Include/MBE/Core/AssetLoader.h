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
#include <MBE/Animation/AnimationHolder.h>

namespace mbe
{

	class AssetLoader
	{
	public:
		template <class TAssetHolder>
		static void Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::TextureWrapperHolder
		static void Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::FilePathDictionary (Music dictionary)
		static void Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::AnimationHolder
		static void Load(AnimationHolder& animationHolder, const std::string& filePath, std::string fileName);

	private:
		static std::vector<std::vector<std::string>> Tokenise(std::ifstream& file);

		static std::string FindFolder(std::vector<std::vector<std::string>>& lineTokens);
	};


#pragma region Template Implementations

	template<class TAssetHolder>
	inline void AssetLoader::Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName)
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
				const auto& assetId = tokens.at(0);
				const auto& op = tokens.at(1);
				const auto& fileString = tokens.at(2);

				// Switch on the operator
				if (op == "=")
					assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString);
				else
					std::cerr << "AssetLoader load asset: invalid declaration" << "(" << fileName << ")" << std::endl;
			}
			else
				std::cerr << "AssetLoader load asset: invalid declaration" << "(" << fileName << ")" << std::endl;
		}
	}

} // namespace mbe

#pragma endregion
