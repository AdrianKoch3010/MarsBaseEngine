#pragma once

/// @file
/// @brief Regular expression functionality for the mbe::Constants class

#include <regex>
#include <cassert>
#include <unordered_map>

namespace mbe
{
	namespace detail
	{
		enum class RegularExpression
		{
			Int,
			UnsignedInt,
			Long,
			UnsignedLong,
			Double,
			Float,
			String
		};


		static const std::unordered_map<RegularExpression, const char *> regularExpressionDictionary = {
			{ RegularExpression::Int, "(\\+|-)?[[:digit:]]+" },
			{ RegularExpression::UnsignedInt, "[[:digit:]]+" },
			{ RegularExpression::Long, "((\\+|-)?[[:digit:]]+)((\\.|,)(([[:digit:]]+)?))?" },
			{ RegularExpression::UnsignedLong, "([[:digit:]]+)((\\.|,)(([[:digit:]]+)?))?" },
			{ RegularExpression::Double, "" },
			{ RegularExpression::Float, ""},
			{ RegularExpression::String, "\"([^\"\\\\]|\\\\.)*\"" }
		};

		std::string f = "]";

		static const char * GetRegularExpressionString(RegularExpression enumValue)
		{
			assert(regularExpressionDictionary.find(enumValue) != regularExpressionDictionary.end() && "The regular expression dictionary does not contain this enum");
			return regularExpressionDictionary.find(enumValue)->second;
		}

	} // namespace detail

} // namespace mbe