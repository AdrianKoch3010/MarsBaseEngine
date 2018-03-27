#pragma once

/// @file
/// @brief Class mbe::FilePathDictionary

#include <string>
#include <unordered_map>

#include "Utility.h"


namespace mbe
{
	/// @brief Wrapper around the std::unordered_map class
	template <typename TID = std::string>
	class FilePathDictionary
	{
	public:
		FilePathDictionary() = default;
		FilePathDictionary(const FilePathDictionary & obj) = default;
		FilePathDictionary(FilePathDictionary && obj) = default;
		FilePathDictionary & operator = (const FilePathDictionary & obj) = default;
		FilePathDictionary & operator = (FilePathDictionary && obj) = default;

	public:
		void Insert(const TID & id, const std::string & filePath);
		// Rvalue reference overload
		void Insert(TID && id, std::string && filePath);

		const std::string & Get(const TID & id) const;

		inline const std::string & operator [] (const TID & id) const { return this->Get(id); }

	private:
		std::unordered_map<TID, std::string> dictionary;
	};

#pragma region Template overload for TID = std::string

	// Normalises the id string
	template<>
	class FilePathDictionary<std::string>
	{
	public:
		FilePathDictionary() = default;
		FilePathDictionary(const FilePathDictionary & obj) = default;
		FilePathDictionary(FilePathDictionary && obj) = default;
		FilePathDictionary & operator = (const FilePathDictionary & obj) = default;
		FilePathDictionary & operator = (FilePathDictionary && obj) = default;

	public:
		void Insert(const std::string & id, const std::string & filePath);
		// Rvalue reference overload
		void Insert(std::string && id, std::string && filePath);

		const std::string & Get(const std::string & id) const;

		inline const std::string & operator [] (const std::string & id) const { return this->Get(id); }

	private:
		std::unordered_map<std::string, std::string> dictionary;
	};

#pragma endregion


#pragma region Template Implementations for FilePathDictionary<TID>

	template<typename TID>
	inline void FilePathDictionary<TID>::Insert(const TID & id, const std::string & filePath)
	{
		dictionary.insert({ id, filePath });
	}

	template<typename TID>
	inline void FilePathDictionary<TID>::Insert(TID && id, std::string && filePath)
	{
		dictionary.insert({ id, filePath });
	}

	template<typename TID>
	inline const std::string & FilePathDictionary<TID>::Get(const TID & id) const
	{
		const auto it = dictionary.find(id);
		if (it == dictionary.cend())
			throw std::runtime_error("FilePathDictionary: No file path exists under this id");
		return it->second;
	}

#pragma endregion

#pragma region Template Implementations for FilePathDictionary<std::string>

	inline void FilePathDictionary<std::string>::Insert(const std::string & id, const std::string & filePath)
	{
		dictionary.insert({ NormaliseIDString(id), filePath });
	}

	inline void FilePathDictionary<std::string>::Insert(std::string && id, std::string && filePath)
	{
		NormaliseIDString(id);
		dictionary.insert({ id, filePath });
	}

	inline const std::string & FilePathDictionary<std::string>::Get(const std::string & id) const
	{
		const auto it = dictionary.find(id);
		if (it == dictionary.cend())
			throw std::runtime_error("FilePathDictionary: No file path exists under this id: " + id);
		return it->second;
	}

#pragma endregion

} // namespace mbe