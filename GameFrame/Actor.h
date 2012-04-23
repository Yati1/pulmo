#ifndef ACTOR_H
#define ACTOR_H

#include <Ogre.h>
#include "PhysicsSystem.h"

//include IDs these Ids are used to identify wwhat the actor is on a collision callback
//#include "AstroidsActorID.h"

//forward declare ActorManager
class ActorManager;
/*****************************************************************************
Actor

Represents a "game entity with physics"

The main feature is the onContact function which fires when this Actor's Critter::Body comes into contact
with any other Body that it is flagged against
*****************************************************************************/
class Actor: public NxOgre::Callback
{
public:
	Actor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager);
	virtual ~Actor(void);

	Critter::Body* getBody()const{return m_pBody;}
	Ogre::Entity* getEntity()const{return m_pEntity;}

	bool isToBeDeleted()const{return m_bToBeDeleted;}

	void setToBeDeleted(bool b){m_bToBeDeleted=b;}

	virtual void update(float fElapsedTime);

	virtual void onContact(const NxOgre::ContactPair& pair);

protected:
	Ogre::Entity* m_pEntity;
	Critter::Body* m_pBody;

	ActorManager* m_pManager;

	bool m_bToBeDeleted;
};
#endif