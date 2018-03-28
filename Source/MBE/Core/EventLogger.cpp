#include <MBE/Core/EventLogger.h>

using namespace mbe;

EventLogger::EventLogger(EventManager & eventManager, std::string filePath) :
	eventManager(eventManager),
	logFile(filePath)
{
}

EventLogger::~EventLogger() {
	logFile.close();
	for (const auto subscription : subscriptions)
	{
		eventManager.UnSubscribe(subscription);
	}
}