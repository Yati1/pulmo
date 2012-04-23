#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "actor.h"
#include "SoundSystem.h"
//#include "AstroidsActorManager.h"
#include "ActorManager.h"
//forward declare
class AstroidsActorManager;

class Projectile :
	public Actor
{
public:
	Projectile(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager);
	~Projectile(void);

	void onContact(const NxOgre::ContactPair& pair);
	void update(float fElapsedTime);

protected:
	float m_fMaxDist;
	Ogre::Vector3 m_v3InitialPos;
};
#endif