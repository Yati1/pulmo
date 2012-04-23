#ifndef DATAEVENT_H
#define DATAEVENT_H

#include "Event.h"


//The data event is just a basic event that provide some data witch type is parameterized.
//Use this class for simple events that just hold some informations.

namespace EventSystem
{
	template< typename DataType >
	class DataEvent : public Event
	{
	public:

		DataEvent( Event::TypeId typeId, DataType d )
			: Event( typeId )
			, m_data( d )
		{}

		// note : here we assume that NRVO will play a big part to help getting directly the data.
		DataType data() const { return m_data; }
		
	protected:
		
	private:
	
		const DataType m_data;

	};

	
	// Utility function to create a data event with it's pointer "on the fly" with the default basic allocator.
	template< typename DataType >
	EventPtr makeDataEvent( Event::TypeId type, DataType d )
	{
		return EventPtr( new DataEvent< DataType >( type, d ) );
	}
}


#endif