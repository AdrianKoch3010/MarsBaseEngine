#pragma once

/// @file
/// @brief Class template mbe::FilePathDictionary and mbe::FilePathDictionary<std::string>

#include <string>
#include <unordered_map>

#include <MBE/Core/Utility.h>


namespace mbe
{
	/// @brief Wrapper around the std::unordered_map<TID, std::string> class
	/// @details Defines methods for inserting and accessing file paths.
	/// There is a template overload for TID == std::string that normalises the id string
	/// @tparam TID Td type that must be usable as an id in a std::unordered_map
	template <typename TID = std::string>
	class FilePathDictionary
	{
	public:
		/// @brief Default constructor
		FilePathDictionary() = default;

		/// @brief Default copy constructor
		FilePathDictionary(const FilePathDictionary & obj) = default;
		
		/// @brief Default move constructor
		FilePathDictionary(FilePathDictionary && obj) = default;

		/// @brief Default copy assignment operator
		FilePathDictionary & operator = (const FilePathDictionary & obj) = default;

		/// @brief Default move assignment operator
		FilePathDictionary & operator = (FilePathDictionary && obj) = default;

	public:
		/// @brief Inserts a new file path into the dictionary by copying the existing one
		/// @details The file path is inserted only if its key is not equivalent to the key of any other element already in the container.
		/// @param id The id that can be used to access the file path later
		/// @param filePath A reference of the file path to insert
		void Insert(const TID & id, const std::string & filePath);

		/// @brief Inserts a new file path into the dictionary by moving it
		/// @details Rvalue reference overload of the mbe::FilePathDictionary::Insert() method.
		/// The file path is inserted only if its key is not equivalent to the key of any other element already in the container.
		/// @param id The id that can be used to access the file path later
		/// @param filePath An rvalue reference of the file path to insert
		void Insert(TID && id, std::string && filePath);

		/// @brief Finds and returns the file path with the respective id
		/// @param id The id of the file path to get
		/// @returns a const reference to the found path
		/// @throws std::runtime_error if the id could not be found
		const std::string & Get(const TID & id) const;

		/// @brief Finds and returns the file path with the respective id
		/// @details Equivalent to calling mbe::FilePathDictionary::Get(id)
		/// @param id The id of the file path to get
		/// @returns a const reference to the found path
		/// @throws std::runtime_error if the id could not be found
		inline const std::string & operator [] (const TID & id) const { return this->Get(id); }

	private:
		std::unordered_map<TID, std::string> dictionary;
	};

#pragma region Template overload for TID = std::string

	/// @brief Wrapper around the std::unordered_map<std::string, std::string> class
	/// @details Defines methods for inserting and accessing file paths.
	/// This is a template overload of the mbe::FilePathDictionary<TID> class where TID == std::string.
	/// @tparam TID Td type that must be usable as an id in a std::unordered_map
	/// @attention The id strings are <b>not</b> case sensitive! This is to reduce the likelyhood of mistyping an id which may cause unwanted behaviour.
	/// Furthermore <b>only ASCII</b> characters should be used.
	template<>
	class FilePathDictionary<std::string>
	{
	public:
		/// @brief Default constructor
		FilePathDictionary() = default;

		/// @brief Default copy constructor
		FilePathDictionary(const FilePathDictionary & obj) = default;

		/// @brief Default move constructor
		FilePathDictionary(FilePathDictionary && obj) = default;

		/// @brief Default copy assignment operator
		FilePathDictionary & operator = (const FilePathDictionary & obj) = default;

		/// @brief Default move assignment operator
		FilePathDictionary & operator = (FilePathDictionary && obj) = default;

	public:
		/// @brief Inserts a new file path into the dictionary by copying the existing one
		/// @details The file path is inserted only if its key is not equivalent to the key of any other element already in the container.
		/// @param id The id that can be used to access the file path later. <b>Only use ASCII strings!</b>
		/// @param filePath A reference of the file path to insert
		void Insert(const std::string & id, const std::string & filePath);

		/// @brief Inserts a new file path into the dictionary by moving it
		/// @details Rvalue reference overload of the mbe::FilePathDictionary::Insert() method.
		/// The file path is inserted only if its key is not equivalent to the key of any other element already in the container.
		/// @param id An rvalue reference of the id that can be used to access the file path later. <b>Only use ASCII strings!</b>
		/// @param filePath An rvalue reference of the file path to insert
		void Insert(std::string && id, std::string && filePath);

		/// @brief Finds and returns the file path with the respective id
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter. <b>Only use ASCII strings!</b>
		/// @returns a const reference to the found path
		/// @throws std::runtime_error if the id could not be found
		const std::string & Get(const std::string & id) const;

		/// @brief Finds and returns the file path with the respective id
		/// @details Equivalent to calling mbe::FilePathDictionary::Get(id)
		/// @param id The id of the asset to get. It is not case sensetive so capital letters do not matter. <b>Only use ASCII strings!</b>
		/// @returns a const reference to the found path
		/// @throws std::runtime_error if the id could not be found
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