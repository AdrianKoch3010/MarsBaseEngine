#pragma once

/// @file
/// @brief Class mbe::detail::BaseEvent

namespace mbe
{
	namespace detail
	{
		/// @brief Base class of every mbe::Event used to generate the type id
		class BaseEvent
		{
		public:
			/// @brief Defines the type of the type id
			typedef size_t TypeID;

		public:
			/// @brief Default constructor
			BaseEvent() = default;

			/// @brief Default destructor
			virtual ~BaseEvent() = default;

		protected:
			/// @brief Returns a new type id for every call of the function
			/// @details This function is called in the the static GetTypeID() method
			/// of the derived event. This guarantees that every event class has is own
			/// unique id.
			static TypeID GetNextID();
		};
	} // namespace detail
} // namespace mbe