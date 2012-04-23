#include "Projectile.h"
//#include "AstroidsActorManager.h"

Projectile::Projectile(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager): Actor(entity,body,manager)
{
	m_fMaxDist = 100.0f;
	m_v3InitialPos=entity->getParentSceneNode()->getPosition();
}

Projectile::~Projectile(void)
{
}

void Projectile::update(float fElapsedTime)
{
	float dist = m_pEntity->getParentSceneNode()->getPosition().distance(m_v3InitialPos);
	if(dist>m_fMaxDist)
		ActorManager::GetSingleton()->queueToDelete(this->getBody());
}

void Projectile::onContact(const NxOgre::ContactPair& pair)
{
	if(!m_bToBeDeleted)
	{
		unsigned int group1,group2;
		group1 = pair.mFirst->getShape(0)->getGroup();
		group2 = pair.mSecond->getShape(0)->getGroup();

		FMOD_VECTOR p,v;
		Ogre::Vector3 pos = m_pEntity->getParentSceneNode()->getPosition();

		p.x = pos.x;
		p.y = pos.y;
		p.z = pos.z;

		v.x=0.0f;
		v.y=0.0f;
		v.z=0.0f;
		SoundSystem::GetSingleton()->play3DSound("explosion-02.wav",p,v,1.0f);
		//SoundSystem::GetSingleton()->playSound("Explosion_CriticalHit2.ogg");
		ActorManager::GetSingleton()->queueToDelete(this->getBody());

		/*
		unsigned int group = pair.mSecond->getShape(0)->getGroup();
		switch(group)
		{
		case GROUP_ID_ASTROIDs:
			SoundSystem::GetSingleton()->playSound("Explosion_CriticalHit2.ogg");

			AstroidsActorManager::GetSingleton()->queueToDelete(this->getBody());

			//Ogre::LogManager::getSingletonPtr()->logMessage("!! PROJECTILE HIT ASTROID !!!");
			break;
		default:
			Ogre::LogManager::getSingletonPtr()->logMessage("!! PROJECTILE HIT !!!");
		}
		*/
	}
}