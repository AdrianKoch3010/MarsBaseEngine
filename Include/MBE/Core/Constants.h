//#pragma once
//
///// @file
///// @brief Class mbe::Constants
//
//#include <iostream> // for debug messages
//#include <vector>
//#include <fstream>  // for reading files
//
//#include <MBE/Core/RegularExpressions.h>
//#include <MBE/Core/Singleton.h>
//
//namespace mbe
//{
//	namespace detail
//	{
//		static const char contentDoesNotMatch[] = "Content does not match";
//	}
//
//	/// @brief Singleton container class that reads in a the constants.txt and sets the constants used throughout the program
//	class Constants : public detail::Singleton<Constants>
//	{
//		friend class detail::Singleton<Constants>;
//
//	protected:
//		Constants();
//
//	public:
//		~Constants();
//
//
//	public:
//		// Returns a flag set in the constructor (where the Load() function is called) that indicated whether there have been any loading errors
//		auto HasLoadedSuccessfully() const { return hasLoadedSuccessfully; }
//
//		inline auto GetScreenWidth() const { return *screenWidth; }
//
//		inline auto GetScreenHeight() const { return *screenHeight; }
//
//		inline auto GetTestTimeMultiplicator() const { return *testTimeMultiplicator; }
//
//		// This defeats the entire purpose of the constants class, but it is obsolete anyway
//		inline void SetTestTimeMultiplicator(float value) { *testTimeMultiplicator = value; }
//
//	protected:
//		bool Load(std::string filePath);
//
//		template <typename TData>
//		bool AddContent(TData** constantToDefine, std::string constantName, detail::RegularExpression expressionToMatch);
//
//		bool ReadFile(std::string filePath);
//
//		std::string FindContent(std::string searchContent, std::string lineToSearch, detail::RegularExpression expressionToMatch);
//
//	private:
//		// The pointer is used to make the values nullable
//		unsigned short int* screenHeight;
//		unsigned short int* screenWidth;
//
//		float* testTimeMultiplicator;
//
//		std::vector<std::string> lines;
//		// A flag (set in the constructor where the Load() function is called) that indicated whether there have been any loading errors
//		bool hasLoadedSuccessfully;
//	};
//} // namespace mbe
//


#pragma once

/// @file
/// @brief class mbe::Constants

#include <iostream> // for debug messages
#include <string>
#include <unordered_map>
#include <variant>

#include <MBE/Core/RegularExpressions.h>
#include <MBE/Core/Singleton.h>
#include <MBE/Core/Utility.h> // For NormaliseIDString

namespace mbe
{
	// a long is a long long and an unsigned long is an unsigned long long to be able to store handle ids
	// Stores the type depending on the regex
	// Convert between types???


	/// @brief Singleton container class that reads in a the constants.txt and sets the constants used throughout the program
	class Constants : public detail::Singleton<Constants>
	{
		friend class detail::Singleton<Constants>;

	private:
		typedef std::variant<
			int,
			unsigned,
			long long,
			unsigned long long,
			float,
			double,
			std::string>
			ValueType;
		typedef std::unordered_map<std::string, ValueType> ValueDictionary;

	private:
		Constants();

	public:
		~Constants() = default;


	public:
		// Returns a flag set in the constructor (where the Load() function is called) that indicated whether there have been any loading errors
		inline bool HasLoadedSuccessfully() const { return hasLoadedSuccessfully; }

		template <typename T>
		T Get(const std::string& id) const;

	private:
		bool Load(const std::string& filePath);

	private:
		ValueDictionary valueDictionary;

		bool hasLoadedSuccessfully;
	};

} // namespace mbe