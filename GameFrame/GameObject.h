
#if !defined(GAMEOBJECT_H)
#define GAMEOBJECT_H

#include "Event.h"
#include "DataEvent.h"
#include "EventListener.h"
#include "EventManager.h"
#include "GameEvents.h"

using EventSystem::EventListener;
using EventSystem::EventManager;
using EventSystem::Event;
using EventSystem::EventPtr;
using EventSystem::DataEvent;

class GameObj : public EventListener
{
public:
	GameObj(int nObjId, EventManager& eventMgr) : EventListener(), m_fHealth(100.0f), m_nID(nObjId)
	{
		eventMgr.addListener(*this, EVENT_ID_SHOOT);		
		eventMgr.addListener(*this, EVENT_ID_GIVEHEALTH);		
	}

protected:
	virtual void catchEvent(const EventPtr& e, EventManager& mgr)
	{
		const Event::TypeId eType = e->type();
			
		if (EVENT_ID_SHOOT == eType)
		{
			Shoot(2);
		}
		else if (EVENT_ID_GIVEHEALTH == eType)
		{
			const DataEvent<HealthEventData>* pDataEvent = static_cast<const DataEvent<HealthEventData>*>(e.get());
			const HealthEventData& healthData = pDataEvent->data();			
			GiveHealth(healthData.fHealthInc);
		}
	}

private:
	void Shoot(int nVal) 
	{ 
		for (int i = 0; i < nVal; i++)
		{
			printf("GameObj%d Bang!\n", m_nID);
			m_fHealth -= 5.0f;
		}
	}

	void GiveHealth(float fHealthInc) 
	{ 
		m_fHealth += fHealthInc;
		printf("GameObj%d - Current Health Level = %.2f\n", m_nID, m_fHealth);
	}
	
private:
	int m_nID;
	float m_fHealth;
};

#endif //GAMEOBJECT_H