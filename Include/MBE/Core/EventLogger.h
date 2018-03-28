#pragma once

/// @file
/// @brief Class mbe::EventLogger

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <chrono>
#include <ctime>

#include <MBE/Core/EventManager.h>
#include <MBE/Core/EnumBitmask.h>

namespace mbe
{
	namespace detail
	{
		/// @brief The minimum number of characters that the time string must be in order to be used by the ctime_s() function
		constexpr unsigned short int MIN_TIME_BUFFER_SIZE = 26;
	} // namespace detail

	/// @brief Monitors an event manager and logs occuring events
	/// @details Note that only one event manager is monitored. When subscribing to an event, the log function(s) will only
	/// be subscribed to the event manager added in the constructor.
	class EventLogger
	{
	public:
		/// @brief Defines the stream to which the log is written to
		/// @details This enum supports bitmask operators. Note that the EnumBitmask.h file may need to be included.
		enum class OutputStream : unsigned int
		{
			// Using bitshift operator (always one bit set to 1)
			None = 0,
			Console = 1 << 0,
			File = 1 << 1,
			Other = 1 << 2
		};

	public:
		/// @brief Constructor
		/// @param eventManager A reference of the mbe::EventManager the logging will monitor
		/// @param filePath The file path to which the loging file is saved to
		EventLogger(EventManager & eventManager, std::string filePath = "Resources/LogFile.txt");

		/// @brief Destructor
		/// @details When the mbe::EventLogger is destroyed the log file is saved and all log functions are unsubscribed
		~EventLogger();

		/// @brief Subscribes a log function to an event type
		/// @tparam TEvent The type of event to be logged
		/// @param outputStream The outputStream(s) to which the logging is written. By default, it is set to the console
		/// @note More than one outputStream can be added by combing multiple streams using the | operator
		template <class TEvent>
		void Subscribe(OutputStream outputStream = OutputStream::Console);

	private:
		EventManager & eventManager;
		std::ofstream logFile;
		std::vector<EventManager::SubscriptionID> subscriptions;
	};

} // namespace mbe

MBE_ENABLE_BITMASK_OPERATORS(mbe::EventLogger::OutputStream)

namespace mbe
{

#pragma region Template Implementation

	template<class TEvent>
	inline void EventLogger::Subscribe(OutputStream outputStream) {
		//EventManager::TCallback<TEvent> logFunction;
		EventManager::SubscriptionID subscription;

		if ((outputStream & OutputStream::Console) != OutputStream::None)
		{
			subscription = eventManager.Subscribe(EventManager::TCallback<TEvent>([](const TEvent & event)
			{
				std::cout << std::endl << event;
			}));
			subscriptions.push_back(subscription);
		}
		if ((outputStream & OutputStream::File) != OutputStream::None)
		{
			subscription = eventManager.Subscribe(EventManager::TCallback<TEvent>([this](const TEvent & event)
			{
				// Get a string of the current time
				time_t now = time(0);
				char time[detail::MIN_TIME_BUFFER_SIZE];
				ctime_s(time, sizeof(time), &now);

				// Remove the std::endl;
				std::string timeString = time;
				timeString.erase(timeString.size() - 1);

				logFile << '[' << timeString << "] " << event << std::endl;
			}));
			subscriptions.push_back(subscription);
		}
	}

#pragma endregion

} // namespace mbe


// There are multiple event managers
// Maybe an overload when only subscribing to a specific event manager
// Have a function to change the outputfile name

///////////////////////////////////////////////////////////////////////
/// @class mbe::EventLogger
/// 
/// <b>Example:</b>
/// @code
/// // Create the event logger (usually there will be one per mbe::EventManager to monitor)
/// mbe::EventLogger eventLogger(eventManager);
/// 
/// // Subscribe to an event
/// eventLogger.Subscribe<MyEvent>();
/// eventLogger.Subscribe<MyEvent>(mbe::EventLogger::OutputStream::File);
///
/// // Bitmask operators can be used to subscribe logging to multiple streams at a time
/// // If MyEvent occures it is logged to both the file and the console
/// eventLogger.Subscribe<MyEvent>(mbe::EventLogger::OutputStream::Console | mbe::EventLogger::OutputStream::File);
/// // If MyEvent occures it is logged to everything but the console 
/// eventLogger.Subscribe<MyEvent>(~mbe::EventLogger::OutputStrean::Console);
/// @endcode
///////////////////////////////////////////////////////////////////////