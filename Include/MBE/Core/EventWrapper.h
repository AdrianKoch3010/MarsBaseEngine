#pragma once

/// @file
/// @brief Class template mbe::detail::EventWrapper

#include <MBE/Core/BaseEvent.h>

namespace mbe
{
	namespace detail
	{
		/// @brief Wrapps an event that can be raised using the mbe::EventManager
		/// @details This allows the TEvent to be casted to a BaseEvent even though it does not inherit from it.
		template <class TEvent>
		class EventWrapper : public detail::BaseEvent
		{
		public:
			/// @brief Default constructor
			/// @param event The event that will be wrapped by this class
			EventWrapper(const TEvent & event);

			/// @brief Default destructor
			virtual ~EventWrapper() = default;

		public:
			/// @brief Returns the type id of the derived event class
			/// @details The type id unique for every event. This works since Event<A> and Event<B> are different types.
			/// This is the reason that classes that inherit from mbe::Event must add itself as the template parameter.
			static TypeID GetTypeID();

			const TEvent & event;
		};

#pragma region Template Implementations

		template<class TEvent>
		inline EventWrapper<TEvent>::EventWrapper(const TEvent & event) : event(event)
		{
		}

		template<class TEvent>
		inline typename EventWrapper<TEvent>::TypeID EventWrapper<TEvent>::GetTypeID()
		{
			static TypeID typeId = detail::BaseEvent::GetNextID();
			return typeId;
		}

#pragma endregion
	} // namespace detail
} // namespace mbe

  // see: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
  // and: https://stackoverflow.com/questions/47337029/handling-function-pointer-with-covariant-types-uniformly-how-to-call-callbacks