#pragma once

/// @file
/// @brief class mbe::Constants

#include <iostream> // for debug messages
#include <string>
#include <unordered_map>
#include <variant>

#include <MBE/Core/Singleton.h>
#include <MBE/Serialisation/ConstantsParser.h>
#include <MBE/Core/Utility.h> // For NormaliseIDString

namespace mbe
{
	// a long is a long long and an unsigned long is an unsigned long long to be able to store handle ids
	// Stores the type depending on the regex
	// Convert between types???


	/// @brief Singleton container class
	class Constants : public detail::Singleton<Constants>
	{
		friend class detail::Singleton<Constants>;

	private:
		typedef ConstantsParser::ValueType ValueType;
		typedef std::unordered_map<std::string, ValueType> ValueDictionary;

	private:
		Constants();

	public:
		~Constants() = default;


	public:
		// Mutiple files can be loaded as long as the ids don't clash
		void Load(const std::string& filePath);

		template <typename T>
		T Get(const std::string& id) const;

		void Add(const std::string& id, ValueType value);

	private:
		ValueDictionary valueDictionary;
	};

#pragma region Template Implementation

	template<typename T>
	inline T Constants::Get(const std::string& id) const
	{
		auto normalId = NormaliseIDString(id);
		auto it = valueDictionary.find(normalId);
		if (it == valueDictionary.end())
			throw std::runtime_error("ConstantsParser: No entry for this id: " + normalId);

		// Check the type -- Some logic for type conversion could be added
		if (!std::holds_alternative<T>(it->second))
			throw std::runtime_error("The entry has been requested with the incorrect type: " + normalId);

		return std::get<T>(it->second);
	}

#pragma endregion


} // namespace mbe