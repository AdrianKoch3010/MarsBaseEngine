#pragma once

/// @file
/// @brief Functionality for typesafe enum bitmasks
/// @details The following link describes the concept used in more detail: http://blog.bitwigglers.org/using-enum-classes-as-type-saf

#include <type_traits>

namespace mbe
{
	// A template specialisation can be used to set the enabled value to true
	template <typename TEnum>
	struct EnableBitmaskOperators
	{
		static constexpr bool enabled = false;
	};

} // namespace mbe
	/// @brief Allows the type x to be used with bitmask operators
	/// @details Adds a template specialisation to the EnableBitmaskOperators struct
	/// and overrides the static enabled member to true. This can be done by hand though the makro
	/// exists to avoid boilerplate code.
	/// @param enum The enum type for which the operators are enabled
#define MBE_ENABLE_BITMASK_OPERATORS(enum)		\
namespace mbe									\
{												\
	template<>									\
		struct EnableBitmaskOperators<enum>		\
	{											\
		static constexpr bool enabled = true;	\
	};											\
}

namespace mbe
{

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type operator | (TEnum lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		return static_cast<TEnum>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type operator & (TEnum lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		return static_cast<TEnum>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type operator ^ (TEnum lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		return static_cast<TEnum>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type operator ~ (TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		return static_cast<TEnum>(~static_cast<Underlying>(rhs));
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type & operator |= (TEnum & lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		lhs = static_cast<TEnum>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
		return lhs;
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type & operator &= (TEnum & lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		lhs = static_cast<TEnum>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
		return lhs;
	}

	template <typename TEnum>
	typename std::enable_if<EnableBitmaskOperators<TEnum>::enabled, TEnum>::type & operator ^= (TEnum & lhs, TEnum rhs)
	{
		static_assert(std::is_enum<TEnum>::value, "The template parameter is not an enum type");
		using Underlying = std::underlying_type<TEnum>::type;
		lhs = static_cast<TEnum>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));
		return lhs;
	}
} // namespace mbe
