#include "EventManager.h"
//#include "AppCommon.h"
#include "assert.hpp"
#include <algorithm>

namespace EventSystem
{
	EventManager::EventManager() : m_bListenerRegisterLocked(false)
	{
		
	}


	EventManager::~EventManager()
	{
		clear();
	}

	void EventManager::addListener( EventListener& eventListener, Event::TypeId typeToCatch )
	{
		BOOST_ASSERT_MSG(!m_bListenerRegisterLocked, "Can't add a listener while processing listener registry");
		if (!m_bListenerRegisterLocked)
		{
			// get the listeners for this event type or create it
			EventListenerList& listeners = m_listenerRegister[ typeToCatch ]; 

			// check
			BOOST_ASSERT_MSG( std::find( listeners.begin(), listeners.end(), &eventListener ) == listeners.end(), "Tried to add the same listener twice for event ");

			// register the listener
			listeners.push_back( &eventListener );
		}
	}

	void EventManager::removeListener( EventListener& eventListener )
	{
		if( m_listenerRegister.empty() )
		{
			BOOST_ASSERT_MSG( !m_listenerRegister.empty(), "Tried to remove a listener from an empty event manager!" );
			return; 
		}

		BOOST_ASSERT_MSG(!m_bListenerRegisterLocked, "Can't remove a listener while processing listener registry");
		if (!m_bListenerRegisterLocked)
		{
			EventListenerRegister::iterator register_it = m_listenerRegister.begin();

			for( ; register_it != m_listenerRegister.end(); ++register_it )
			{
				EventListenerList& listeners = register_it->second;

				if(listeners.empty())
					continue; // Skip empty listeners.

				listeners.remove( &eventListener );
			}
		}

	}

	void EventManager::removeListener(  Event::TypeId typeToCatch, EventListener& eventListener )
	{
		BOOST_ASSERT_MSG(!m_bListenerRegisterLocked, "Can't remove a listener while processing listener registry");
		if (!m_bListenerRegisterLocked)
		{
			EventListenerRegister::iterator it = m_listenerRegister.find( typeToCatch );

			BOOST_ASSERT_MSG( it != m_listenerRegister.end(), "Tried to remove listener from unregistered event type : ");

			if( it != m_listenerRegister.end() )
			{
				EventListenerList& listeners = it->second;
				listeners.remove( &eventListener );
			}
		}
	}

	void EventManager::removeAllListeners()
	{
		BOOST_ASSERT_MSG(!m_bListenerRegisterLocked, "Can't remove a listener while processing listener registry");
		if (!m_bListenerRegisterLocked)
		{
			m_listenerRegister.clear();
		}
	}

	void EventManager::clear()
	{
		BOOST_ASSERT_MSG(!m_bListenerRegisterLocked, "Can't remove a listener while processing listener registry");
		if (!m_bListenerRegisterLocked)
		{
			removeAllListeners();
		}
	}

	void EventManager::broadcastEvent( const EventPtr& e , bool bImmediate /*=true*/)
	{
		BOOST_ASSERT( e.get() );

		if (bImmediate)
		{
			processEvent( e );
		}
		else
		{
			m_eventQueue.push_back( e );
		}

	}

	void EventManager::processEventQueue()
	{
		if( m_eventQueue.empty() ) return;

		// First we have to swap the current event queue with the process one ( which will be empty).
		// We do this so that if any events cause other events to be added to the queue, they'll be processed
		// next time we run the function.
		m_processEventQueue.clear();
		m_processEventQueue.swap( m_eventQueue ); // ready for the next event sent

		// now we have to process each event in the processing queue
		for( EventQueue::const_iterator it = m_processEventQueue.begin(); it != m_processEventQueue.end(); ++it )
		{
			const EventPtr e = *it;
			BOOST_ASSERT( e.get() );
			processEvent( e );
		}

	}

	void EventManager::processEvent( const EventPtr& e )
	{
		m_bListenerRegisterLocked = true;
		BOOST_ASSERT( e.get() );

		if( m_listenerRegister.empty() ) return; // no listener registered at all!

		// first dispatch this event to listeners registered to listen to all events
		EventListenerList& listenAllList = m_listenerRegister[ Event::TypeId() ];
		dispatchEvent( e, listenAllList );

		// retrieve the event listener list for this event type if it exists
		EventListenerRegister::iterator register_it = m_listenerRegister.find( e->type() );

		if( register_it == m_listenerRegister.end() ) return; // no listeners for this event!
		
		// now dispatch the event to those listeners waiting for it
		EventListenerList& listeners = register_it->second;
		dispatchEvent( e, listeners );
		m_bListenerRegisterLocked = false;
	}

	void EventManager::dispatchEvent( const EventPtr& e, EventListenerList& listeners )
	{
		BOOST_ASSERT( e.get() );

		if(listeners.empty())
			return;

		// Send the event to all those catchers
		for (EventListenerList::const_iterator it = listeners.begin(); it != listeners.end(); ++it)
		{
			EventListener* pListener = *it;
			pListener->catchEvent(e, *this);
		}
	}
}