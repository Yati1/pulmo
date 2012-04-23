#ifndef BASEEVENT_H
#define BASEEVENT_H

#include <memory>		// For shared ptr
#include <string>

namespace EventSystem
{
	/** Event object base class.
		
		An Event is sent in an EventManager that will reroute
		it to it's registered EventListeners that listen to the same TypeId.
		@par
		This class alone is enough to manage simple events. In the case
		of an event that should provide data, you should use this class as
		base class.
		@remark
		An event can sent to the appropriate EventListeners in an immediate way 
		or in a buffered way. See EventManager class for more details.
		@see EventManager

	*/
	class Event
	{
	public:

		///Event type id.
		typedef std::string TypeId;

		/** Constructor.
			@param type Type of this event.
		*/
		Event( TypeId type )
			:m_type(type)
		{}
		
		virtual ~Event(){}

		/** Event type id value.
			@return Type id of this event.
		*/
		TypeId type() const {return m_type;}
	
	private:

		///Event type value. It is not constant to make the Event class serializable if necessary.
		const TypeId m_type;

	};

	/** Smart pointer for Event objects. */
	typedef  std::tr1::shared_ptr<Event> EventPtr;

	/** Utility function to create a simple event with it's pointer "on the fly" with the default basic allocator.*/
	EventPtr makeEvent( Event::TypeId type );
}
#endif