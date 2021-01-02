#pragma once

/// @file
/// @brief class mbe::AssetLoader


#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

#include <MBE/Core/AssetHolder.h>
#include <MBE/Core/FilePathDictionary.h>
#include <MBE/Animation/AnimationHolder.h>

#include <MBE/Core/Exceptions.h>

namespace mbe
{

	class AssetLoader
	{
	private:
		typedef std::vector<std::pair<std::vector<std::string>, size_t>> TokenLineList;

	public:
		// Throws a std::runntime_error when the file can't be opened
		// Throws a mbe::ParseError for invalid syntax
		template<class TAssetHolder>
		static void Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName);

		// Attempts to load the assets, if an error occurs no assets are loaded and the error message is output to std::err
		template<class TAsserHolder>
		static void TryLoad(TAsserHolder& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::TextureWrapperHolder
		template<>
		static void Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::FilePathDictionary (Music dictionary)
		template<>
		static void Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName);

		// Overload for mbe::AnimationHolder
		template<>
		static void Load(AnimationHolder& animationHolder, const std::string& filePath, const std::string& fileName);

	private:
		// Throws when the file can't be opened or when Tokenise() or FindFolder() throws
		static std::pair<TokenLineList, std::string> LoadTokensAndFile(const std::string& filePath, const std::string fileName);

		static TokenLineList Tokenise(std::ifstream& file);

		static std::string FindFolder(TokenLineList& lineTokens);
	};


#pragma region Template Implementations

	template<class TAssetHolder>
	inline void AssetLoader::Load(TAssetHolder& assetHolder, const std::string& filePath, const std::string& fileName)
	{
		const auto pair = LoadTokensAndFile(filePath, fileName);
		const auto& lineTokens = pair.first;
		const auto& folder = pair.second;

		std::string currentSubFolder = "";
		for (const auto& tokensPair : lineTokens)
		{
			const auto& tokens = tokensPair.first;
			const auto lineNumber = tokensPair.second;

			// If this is a subfolder declaration
			if (tokens.size() == 1 && tokens.front().front() == '[')
			{
				if (tokens.front().at(tokens.front().size() - 1) != ']')
					throw ParseError(MBE_NAME_OF(AssetLoader), "Subfolder declaration must be enclosed by []", lineNumber);

				currentSubFolder = tokens.front();
				// Remove the brackets
				currentSubFolder.erase(0, 1);
				currentSubFolder.erase(currentSubFolder.size() - 1, 1);
			}
			// If this is an asset declaration
			else if (tokens.size() >= 3)
			{
				// The first 3 token shall not contain any comments
				if (tokens[0].find('#') != std::string::npos
					|| tokens[1].find('#') != std::string::npos
					|| tokens[2].find('#') != std::string::npos)
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);

				// If there are more than 3 tokens the following tokens must be comments
				if (tokens.size() > 3 && tokens[3].front() != '#')
					throw ParseError(MBE_NAME_OF(AssetHolder), "An asset declaration can only be followed by a comment", lineNumber);

				const auto& assetId = tokens.at(0);
				const auto& op = tokens.at(1);
				const auto& fileString = tokens.at(2);

				// Switch on the operator
				if (op == "=")
					assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString);
				else
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid operator", lineNumber);
			}
			else
				throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);
		}
	}

	template<class TAsserHolder>
	inline void AssetLoader::TryLoad(TAsserHolder& assetHolder, const std::string& filePath, const std::string& fileName)
	{
		try
		{
			Load<TAsserHolder>(assetHolder, filePath, fileName);
		}
		catch (const ParseError& parseError)
		{
			std::cerr << filePath + fileName << ": " << parseError.GetParser() << ":" << std::endl;
			if (parseError.GetLineNumber() < 0)
				std::cerr << fileName << ": " << parseError.GetMessage() << "\n" << std::endl;
			else
				std::cerr << fileName << ":" << parseError.GetLineNumber() << ": " << parseError.GetMessage() << "\n" << std::endl;
		}
		catch (const std::runtime_error& error)
		{
			std::cerr << error.what() << std::endl;
		}
	}

	template<>
	void AssetLoader::Load(TextureWrapperHolder<>& assetHolder, const std::string& filePath, const std::string& fileName)
	{
		const auto pair = LoadTokensAndFile(filePath, fileName);
		const auto& lineTokens = pair.first;
		const auto& folder = pair.second;

		std::string currentSubFolder = "";
		for (const auto& tokensPair : lineTokens)
		{
			const auto& tokens = tokensPair.first;
			const auto lineNumber = tokensPair.second;

			// If this is a subfolder declaration
			if (tokens.size() == 1 && tokens.front().front() == '[')
			{
				// A subfolder declaration must end on a ']'
				if (tokens.front().at(tokens.front().size() - 1) != ']')
					throw ParseError(MBE_NAME_OF(AssetLoader), "Subfolder declaration must be enclosed by []", lineNumber);

				currentSubFolder = tokens.front();
				// Remove the brackets
				currentSubFolder.erase(0, 1);
				currentSubFolder.erase(currentSubFolder.size() - 1, 1);
			}
			// If this is an asset declaration
			else if (tokens.size() >= 3)
			{
				// The first 3 token shall not contain any comments
				if (tokens[0].find('#') != std::string::npos
					|| tokens[1].find('#') != std::string::npos
					|| tokens[2].find('#') != std::string::npos)
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);

				// If there are more than 3 tokens the following tokens must be comments
				if (tokens.size() > 3 && tokens[3].front() != '#')
					throw ParseError(MBE_NAME_OF(AssetHolder), "An asset declaration can only be followed by a comment", lineNumber);

				const auto& assetId = tokens.at(0);
				const auto& op = tokens.at(1);
				const auto& fileString = tokens.at(2);

				// Switch on the operator
				if (op == "=")
					assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString, false);
				else if (op == "*=")
					assetHolder.Load(assetId, filePath + folder + currentSubFolder + fileString, true);
				else
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid operator", lineNumber);
			}
			else
				throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);
		}
	}

	template<>
	void AssetLoader::Load(FilePathDictionary<>& filePathDictionary, const std::string& filePath, const std::string& fileName)
	{
		const auto pair = LoadTokensAndFile(filePath, fileName);
		const auto& lineTokens = pair.first;
		const auto& folder = pair.second;

		std::string currentSubFolder = "";
		for (const auto& tokensPair : lineTokens)
		{
			const auto& tokens = tokensPair.first;
			const auto lineNumber = tokensPair.second;

			// If this is a subfolder declaration
			if (tokens.size() == 1 && tokens.front().front() == '[')
			{
				if (tokens.front().at(tokens.front().size() - 1) != ']')
					throw ParseError(MBE_NAME_OF(AssetLoader), "Subfolder declaration must be enclosed by []", lineNumber);

				currentSubFolder = tokens.front();
				// Remove the brackets
				currentSubFolder.erase(0, 1);
				currentSubFolder.erase(currentSubFolder.size() - 1, 1);
			}
			// If this is an asset declaration
			else if (tokens.size() >= 3)
			{
				// The first 3 token shall not contain any comments
				if (tokens[0].find('#') != std::string::npos
					|| tokens[1].find('#') != std::string::npos
					|| tokens[2].find('#') != std::string::npos)
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);

				// If there are more than 3 tokens the following tokens must be comments
				if (tokens.size() > 3 && tokens[3].front() != '#')
					throw ParseError(MBE_NAME_OF(AssetHolder), "A declaration can only be followed by a comment", lineNumber);

				const auto& id = tokens.at(0);
				const auto& op = tokens.at(1);
				const auto& fileString = tokens.at(2);

				// Switch on the operator
				if (op == "=")
					filePathDictionary.Insert(id, filePath + folder + currentSubFolder + fileString);
				else
					throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid operator", lineNumber);
			}
			else
				throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);
		}
	}

	template<>
	void AssetLoader::Load(AnimationHolder& animationHolder, const std::string& filePath, const std::string& fileName)
	{
		const auto pair = LoadTokensAndFile(filePath, fileName);
		const auto& lineTokens = pair.first;
		const auto& folder = pair.second;

		std::string currentSubFolder = "";
		for (const auto& tokensPair : lineTokens)
		{
			const auto& tokens = tokensPair.first;
			const auto lineNumber = tokensPair.second;

			// If this is a subfolder declaration
			if (tokens.size() == 1 && tokens.front().front() == '[')
			{
				if (tokens.front().at(tokens.front().size() - 1) != ']')
					throw ParseError(MBE_NAME_OF(AssetLoader), "Subfolder declaration must be enclosed by []", lineNumber);

				currentSubFolder = tokens.front();
				// Remove the brackets
				currentSubFolder.erase(0, 1);
				currentSubFolder.erase(currentSubFolder.size() - 1, 1);
			}
			// If this is an asset declaration
			else if (tokens.size() >= 1)
			{
				// The second token must be a comment
				if (tokens.size() > 1 && tokens[1].front() != '#')
					throw ParseError(MBE_NAME_OF(AssetLoader), "An animation declaration can only be followed by a comment", lineNumber);

				const auto& animationFileName = tokens.at(0);
				animationHolder.Load(filePath + folder + currentSubFolder + animationFileName);
			}
			else
				throw ParseError(MBE_NAME_OF(AssetLoader), "Invalid declaration", lineNumber);
		}
	}

} // namespace mbe

#pragma endregion
