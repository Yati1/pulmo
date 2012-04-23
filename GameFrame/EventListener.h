#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <functional>

#include "Event.h"
namespace EventSystem
{
	class EventManager;

	/** Receive and manage specific Events.
		
		An EventListener object must be registered in an EventManager to receive
		the Events that the EventManager will receive.
		The EventManager will register the EventListener to catch specific
		event types.
		
		@remark
		This is a virtual base class : the heritor class must define 
		the management of the Event received.
		
		@remark
		An EventListener can be registered by more than one EventManager and
		can listen to more than one type of events.

		@see EventManager, Event

	*/
	class EventListener
	{
	public:

		/** User defined Event reception.
			This method will be called when an Event of the type of 
			processed by the EventManager.
			@param e Event that have been sent.
			@param source EventManager that processed the event.
		*/
		virtual void catchEvent(const EventPtr& e, EventManager& source) = 0;

		virtual ~EventListener() {}
		
	protected:

	private:

	};

	/// Function-like object that can catch events.
	typedef std::tr1::function< void (const EventPtr& , EventManager& ) > EventListenerFunction;

	/** Proxy event listener that only redirect event catches to a provided function-like object.*/
	class ProxyEventListener : public EventListener
	{
	public:
		ProxyEventListener( const EventListenerFunction& catcherFunction )
			: m_catcherFunction( catcherFunction )
		{}

		inline void catchEvent(const EventPtr& e, EventManager& source)
		{
			m_catcherFunction( e, source );
		}

	private:
		
		EventListenerFunction m_catcherFunction;
	};
}

#endif