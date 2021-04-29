#include <MBE/Core/EventManager.h>

using namespace mbe;

void EventManager::UnSubscribe(SubscriptionID subscriptionId)
{
	// There is no way to get the type id of the event for which the function has been subscibed to
	// Therefore, the entire callback dictionary must be searched

	for (auto& callbackDictionary : callbackDictionaryDictionary)
	{
		// Erase by key
		callbackDictionary.erase(subscriptionId);
		// Delete the wrapper (this will not be necessary when using unique pointers)
		//delete BaseCallbackWrapper::GetObjectFromID(subscriptionId);
		delete subscriptionId.GetObjectPtr();
	}
}
