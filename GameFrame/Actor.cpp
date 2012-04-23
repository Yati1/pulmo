#include "Actor.h"

Actor::Actor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager):
	m_pEntity(entity),
	m_pBody(body),
	m_pManager(manager),
	m_bToBeDeleted(false)
{
 	m_pBody->setContactCallback(this);
}

Actor::~Actor(void)
{
}

void Actor::update(float fElapsedTime)
{
	//do nothing
}

void Actor::onContact(const NxOgre::ContactPair& pair)
{
	//do nothing
}