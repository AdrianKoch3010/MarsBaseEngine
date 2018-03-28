#pragma once

/// @file
/// @brief Class mbe::Constants

#include <iostream> // for debug messages
#include <vector>
#include <fstream>  // for reading files

#include <MBE/Core/RegularExpressions.h>
#include <MBE/Core/Singleton.h>

namespace mbe
{
	namespace detail
	{
		static const char contentDoesNotMatch[] = "Content does not match";
	}

	/// @brief Singleton container class that reads in a the constants.txt and sets the constants used throughout the program
	class Constants : public detail::Singleton<Constants>
	{
		friend class detail::Singleton<Constants>;

	protected:
		Constants();

	public:
		~Constants();


	public:
		// Returns a flag set in the constructor (where the Load() function is called) that indicated whether there have been any loading errors
		auto HasLoadedSuccessfully() { return hasLoadedSuccessfully; }

		inline auto GetScreenWidth() { return *screenWidth; }
		inline auto GetScreenHeight() { return *screenHeight; }
		inline auto GetTestTimeMultiplicator() { return *testTimeMultiplicator; }

	protected:
		bool Load(std::string filePath);

		template <typename TData> bool AddContent(TData** constantToDefine, std::string constantName, detail::RegularExpression expressionToMatch);
		bool ReadFile(std::string filePath);
		std::string FindContent(std::string searchContent, std::string lineToSearch, detail::RegularExpression expressionToMatch);

	private:
		// The pointer is used to make the values nullable
		unsigned short int * screenHeight;
		unsigned short int * screenWidth;

		float * testTimeMultiplicator;

		std::vector<std::string> lines;
		// A flag (set in the constructor where the Load() function is called) that indicated whether there have been any loading errors
		bool hasLoadedSuccessfully;
	};
} // namespace mbe

