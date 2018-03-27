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
		enum class RegularExpression : unsigned int
		{
			integer,
			positveInteger,
			decimal,
			positiveDecimal
		};


		static const std::unordered_map<RegularExpression, const char *> regularExpressionDictionary = {
			{ RegularExpression::integer, "(\\+|-)?[[:digit:]]+" },
			{ RegularExpression::positveInteger, "[[:digit:]]+" },
			{ RegularExpression::decimal, "((\\+|-)?[[:digit:]]+)((\\.|,)(([[:digit:]]+)?))?" },
			{ RegularExpression::positiveDecimal, "([[:digit:]]+)((\\.|,)(([[:digit:]]+)?))?" }
		};

		static const char * GetRegularExpressionString(RegularExpression enumValue)
		{
			assert(regularExpressionDictionary.find(enumValue) != regularExpressionDictionary.end() && "The regular expression dictionary does not contain this enum");
			return regularExpressionDictionary.find(enumValue)->second;
		}
	} // namespace detail
} // namespace mbe