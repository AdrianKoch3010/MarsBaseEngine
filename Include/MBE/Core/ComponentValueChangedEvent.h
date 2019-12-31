#pragma once

/// @file
/// @brief Template class mbe::ComponentValueChangedEvent

#include <iostream>
#include <string>

#include <MBE/Core/Utility.h>

namespace mbe
{
	namespace event
	{
		// This event should be raised in the component setters
		// The parameter value is the name of the value changed
		// Note, all names are normalsied
		// Using MBE_NAME_OF from Utility.h can make this easier
		// This class can not be printed by std::ostream
		template <class TComponent>
		class ComponentValueChangedEvent
		{
		public:
			/// @brief Constructor
			/// @param component A reference to the component whose value has been changed
			/// @param value The name of the property that has been changed
			ComponentValueChangedEvent(TComponent& component, std::string value = "Default");

			/// @brief Default Destructor
			~ComponentValueChangedEvent() = default;

		public:

			void SetValue(const std::string & value);

			void SetValue(std::string && value);

			// Returns the normalised value string
			inline const std::string & GetValue() const { return value; }

			// Prefered over GetValue since the passed in value is always normalised
			bool IsValueChanged(const std::string & value) const;

			// Prefered over GetValue since the passed in value is always normalised
			bool IsValueChanged(std::string && value) const;

			// const overload
			const TComponentSerialiser & GetComponent() const { return component; };

			///// @brief Allows this class to be written to an out stream
			///// @details This may be used to output the event's data to the console or a log file
			//friend std::ostream & operator << (std::ostream & stream, const ComponentValueChangedEvent & event);

		private:
			TComponentSerialiser & component;
			std::string value;
		};

#pragma region Template Implementations

			template<class TComponent>
			inline ComponentValueChangedEvent<TComponent>::ComponentValueChangedEvent(TComponent& component, std::string value) :
				component(component),
				value(NormaliseIDString(std::move(value)))
			{
			}

			template<class TComponent>
			inline void ComponentValueChangedEvent<TComponent>::SetValue(const std::string & value)
			{
				NormaliseIDString(value);
				this->value = value;
			}

			template<class TComponent>
			inline void ComponentValueChangedEvent<TComponent>::SetValue(std::string && value)
			{
				this->value = std::move(NormaliseIDString(std::move(value)));
			}

			template<class TComponent>
			inline bool ComponentValueChangedEvent<TComponent>::IsValueChanged(const std::string & value) const
			{
				NormaliseIDString(value);
				return this->value == value;
			}

			template<class TComponent>
			inline bool ComponentValueChangedEvent<TComponent>::IsValueChanged(std::string && value) const
			{
				return this->value == NormaliseIDString(std::move(value));
			}

#pragma endregion

	} // namespace event
} // namespace mbe