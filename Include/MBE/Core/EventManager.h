#pragma once

/// @file
/// @brief Class mbe::EventManager

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <cassert>
#include <SFML\System\NonCopyable.hpp>

#include <MBE/Core/BaseEvent.h>
#include <MBE/Core/EventWrapper.h>
#include <MBE/Core/HandleBase.h>

namespace mbe
{
	/// @brief The core communication system
	/// @details Communication works through raising events and passing an event parameter that can be used to pass event specific information.
	/// Anyone can listen out for events through subscribing to an event of a specific type. The subscirbed callback function will
	/// be called every time an event of that type is raised.
	/// @attention Make sure that the mbe::EventManager is deleted after all the objects that keep references to it to avoid invalid refernces.
	/// Such can cause undefined behaviour or exceptions. An example of this would be the unsubscribing of function objects
	/// using a refernce of an event manager that no longer exists.
	class EventManager : private sf::NonCopyable
	{
	public:
		/// @brief Defines the signature a callback function of type TEvent must have
		/// @tparam The type of event that will be passed as an argument to the callback.
		/// This can be a mbe::BaseEvent or any derived class
		template <class TEvent>
		using TCallback = std::function<void(const TEvent&)>;

#pragma region Private Local Classes
	private:
		class BaseCallbackWrapper : public HandleBase<BaseCallbackWrapper>
		{
		public:
			BaseCallbackWrapper() = default;
			virtual ~BaseCallbackWrapper() = default;

		public:
			virtual void operator() (const detail::BaseEvent& event) = 0;
		};

		template <class TEvent>
		class CallbackWrapper : public BaseCallbackWrapper
		{
		public:
			CallbackWrapper(TCallback<TEvent> callback);
			~CallbackWrapper() = default;

		public:
			void operator() (const detail::BaseEvent& event);

		private:
			TCallback<TEvent> callback;
		};
#pragma endregion

	public:
		// This typedef must be defined after the BaseCallbackWrapper class
		/// @brief Defines the id type used to uniquely identify the CallbackWrapper<TEvent>.
		/// @details This is simply for convenience. It defines the same type as the mbe::HandleBase::ID.
		/// @see mbe::HandleBase::ID
		typedef BaseCallbackWrapper::ID SubscriptionID;

	public:
		/// @brief Default constructor
		EventManager() = default;

		/// @brief Default destructor
		~EventManager() = default;

		/// @brief Adds the passed in callback function to the callback dictionary of the event type
		/// @details The callback function will be called for each raised event of the registered type.
		/// Note that no explicit subscription type must be passed. It is determined implicitly through
		/// the template parameter. A function of signature void(const MyEvent &) will be called whenever
		/// an event of type MyEvent is raised.
		/// @tparam TEvent The type of the event to which the callback function is subscribed to.
		/// This is also the event type passed to the callback function. Therefore, it must not be explicitly stated.
		/// @param callbackFunction A reference to the callback function to add to the callback dictionary.
		/// @returns An id of type SubscriptionID that can be used to unsubscribe the callback function.
		/// If the same function is subscibed more than once, different ids will be returned.
		/// @attention If the subscription id is not stored there is no way to unsubscribe this callback later.
		template <class TEvent>
		SubscriptionID Subscribe(TCallback<TEvent> callbackFunction);

		/// @brief Executes all the callback functions that are subscribed to the event and passes the event to them
		/// @details The template type is used internally to call only the callbacks subscribed to this event's type.
		/// @tparam TEvent The type of event that is raised. This can be any class.
		/// @param event The reference of the event that is raised
		template <class TEvent>
		void RaiseEvent(TEvent& event);

		/// @brief Removes the passed in callback function from the callback dictionary
		/// @attention Opposite to the render systems render nodes every function that is subscribed must be unsubscribed. Not doing
		/// so may lead to undefined behaivour and exceptions and can be difficult do debug. Therefore, a subscription id should be
		/// stored when subscribing a function.
		/// @details Unsubscribing the same callback more than once, although not recommened, will not through an error.
		/// The callback dictionary will be searched for the function, if it is not found nothing will happen.
		/// Note that this method can fail, however, when an invalid reference of the event manager is kept.
		/// @tparam TEvent The type of the event for which the callback function has been subscribed to.
		/// @note This is the more efficient overload of the UnSubscribe() method that should be used when event type is known.
		/// @param subscriptionId The subscription id that referres to the callback function to unsubscribe
		template <class TEvent>
		void UnSubscribe(SubscriptionID subscriptionId);

		/// @brief Removes the passed in callback function from the callback dictionary
		/// @attention Opposite to the render systems render nodes every function that is subscribed must be unsubscribed. Not doing
		/// so may lead to undefined behaivour and exceptions and can be difficult do debug. Therefore, a subscription id should be
		/// stored when subscribing a function.
		/// @details Unsubscribing the same callback more than once, although not recommened, will not through an error.
		/// The callback dictionary will be searched for the function, if it is not found nothing will happen.
		/// Note that this method can fail, however, when an invalid reference of the event manager is kept.
		/// @note This is the less efficient overload of the UnSubscribe() method that should be used when the type of the event
		/// that has been subscribed to is not known.
		/// @param subscriptionId The subscription id that referres to the callback function to unsubscribe
		void UnSubscribe(SubscriptionID subscriptionId);


	private:
		std::vector<std::unordered_map<SubscriptionID, BaseCallbackWrapper*>> callbackDictionaryDictionary;
	};

#pragma region Template Implementations

	template<class TEvent>
	inline typename EventManager::SubscriptionID EventManager::Subscribe(TCallback<TEvent> callback)
	{
		// Get the key of the callback list this callback will be added to
		//DerivedEvent::TypeID typeId = DerivedEvent::GetTypeID();
		typename detail::EventWrapper<TEvent>::TypeID typeId = detail::EventWrapper<TEvent>::GetTypeID();

		// Create a new CallbackWrapper
		BaseCallbackWrapper* baseCallbackPtr = new CallbackWrapper<TEvent>(callback);
		//std::unique_ptr<BaseCallBackWrapper> auto baseCallbackPtr = std::make_unique<CallbackWrapper<DerivedEvent>>(callback);

		// Get the handle id for this callback (this can be used to unsubscribe this callback)
		BaseCallbackWrapper::ID callbackWrapperHandleId = baseCallbackPtr->GetHandleID();

		// This is needed since the [] operator (unlike the std::map) does not
		// create a new item automatically. To achieve that, push_back would have
		// to be called for the first vector as well
		// Resize the vector if necessary
		if (typeId >= callbackDictionaryDictionary.size())
			callbackDictionaryDictionary.resize(typeId + 1);

		// Add the callback to the respective callback list
		//callbackDictionaryList[typeId].insert(std::make_pair<BaseCallBackWrapper::ID, std::unique_ptr<BaseCallBackWrapper>>(callbackWrapperHandleId, std::move(baseCallbackPtr)));
		callbackDictionaryDictionary[typeId].insert(std::make_pair(callbackWrapperHandleId, baseCallbackPtr));

		// return the handle id
		return callbackWrapperHandleId;
	}

	template<class TEvent>
	inline void EventManager::RaiseEvent(TEvent& event)
	{
		// Get the type of the event and therefore the index in the callbackListList
		//DerivedEvent::TypeID typeId = DerivedEvent::GetTypeID();
		typename detail::EventWrapper<TEvent>::TypeID typeId = detail::EventWrapper<TEvent>::GetTypeID();

		// This is needed since the [] operator (unlike the std::map) does not
		// create a new item automatically. To achieve that, push_back would have
		// to be called for the first vector as well
		// Resize the vector if necessary - this may be necessary when an event is risen
		// That has not been subscribed to before (The event manager will not have reserved
		// space for it)
		if (typeId >= callbackDictionaryDictionary.size())
			callbackDictionaryDictionary.resize(typeId + 1);

		// Call all the registered callbacks (for this event type)

		// use indirection to call the event
		// The event can't be directly be passed to the BaseCallbackWrapper since TEvent does not inherit from EventWrapper / BaseEvent
		detail::EventWrapper<TEvent> eventWrapper(event);
		for (auto& callbackPair : callbackDictionaryDictionary[typeId])
		{
			(*callbackPair.second)(eventWrapper);
		}
	}

	template<class TEvent>
	inline void EventManager::UnSubscribe(SubscriptionID subscriptionId)
	{
		// Get the type id of the callback list from which the callback shall be unsubscribed
		//DerivedEvent::TypeID typeId = DerivedEvent::GetTypeID();
		typename detail::EventWrapper<TEvent>::TypeID typeId = detail::EventWrapper<TEvent>::GetTypeID();

		// Erase by key
		callbackDictionaryDictionary[typeId].erase(subscriptionId);

		// Delete the wrapper (this will not be necessary when using unique pointers)
		//delete BaseCallbackWrapper::GetObjectFromID(subscriptionId);
		delete subscriptionId.GetObjectPtr();
	}

#pragma endregion


#pragma region Local Class Template Implementations

	template<class TEvent>
	inline EventManager::CallbackWrapper<TEvent>::CallbackWrapper(TCallback<TEvent> callback) :
		callback(callback)
	{
	}

	template<class TEvent>
	inline void EventManager::CallbackWrapper<TEvent>::operator()(const detail::BaseEvent& event)
	{
		callback(static_cast<const detail::EventWrapper<TEvent>&>(event).event);
	}

#pragma endregion

} //namespace mbe


///////////////////////////////////////////////////////////////////////
/// @class mbe::EventManager
///
/// <b>Example:</b>
/// @code
/// // Create the eventManager (Usually there will be one per application or state)
/// mbe::EventManager eventManager;
///
/// // Subscribe a function of the signature std::function<void(const MyEvent &)>
/// // When an event of type MyEvent is called all subscribed functions of this signature will be called automatically
/// mbe::EventManager::SubscriptionID subscriptionId = eventManager.Subscribe(mbe::EventManager::TCallback<MyEvent>([](const MyEvent & event)
///		{
///			// Do something
///			std::cout << "My event has occured!";
///			// Get information from the event
///			MyData data = event.GetData();
///		}));
///
///	// Create an instance of MyEvent
/// MyEvent myEvent;
///
/// // This will call the above subscribed function
/// eventManager.Raise(myEvent);
///
/// // Unsubscribe the callback function using the subscribtion id
/// eventManager.UnSubscribe<MyEvent>(subscriptionId);
/// @endcode
///////////////////////////////////////////////////////////////////////