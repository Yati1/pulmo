#include "ActorManager.h"
#include "assert.hpp"

ActorManager* Simpleton<ActorManager>::s_singleton=0;

ActorManager::ActorManager(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys):
	m_pSceneManager(pSceneMan),
	m_pPhyxScene(pScene),
	m_pPhyXRenderSys(pRenderSys),
	m_bActorsDataLocked(false)
{
	m_Actors.clear();
	m_tempNewActors.clear();
}

ActorManager::~ActorManager(void)
{
	clearAll();
}

void ActorManager::init(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys)
{
	m_pSceneManager=pSceneMan;
	m_pPhyxScene=pScene;
	m_pPhyXRenderSys=pRenderSys;
	m_bActorsDataLocked=false;
}

void ActorManager::clearAll()
{
	if(!m_Actors.empty())
	{
		ActorsList::iterator iter=m_Actors.begin();

		for(;iter!=m_Actors.end();iter++)
		{
			if(*iter)
			{
				delete *iter;
			}
		}
	}

	m_Actors.clear();
}

void ActorManager::addActor(Actor* toAdd)
{
	if(!m_bActorsDataLocked)
		m_Actors.push_back(toAdd);
	else
		m_tempNewActors.push_back(toAdd);
}

Actor* ActorManager::getActor(Actor* toGet)const
{
	ActorsList::const_iterator iter=std::find(m_Actors.begin(),m_Actors.end(),toGet);
	if(iter!=m_Actors.end())
	{
		return *iter;
	}else
		return 0;
}

Actor* ActorManager::getActor(Critter::Body* toGet)const
{
	ActorsList::const_iterator iter;
	for(iter = m_Actors.begin(); iter!= m_Actors.end(); iter++)
	{
		if((*iter)->getBody()->getNameHash() == toGet->getNameHash())
		{
			return *iter;
		}
	}
	return 0;
}

Actor* ActorManager::createActor(Ogre::Entity* ent,Critter::Body* bod)
{
	Actor* act=new Actor(ent,bod,this);
	addActor(act);
	return act;
}

Actor* ActorManager::createActor(Ogre::Entity* ent,NxOgre::ShapeDescription shapeD,Critter::BodyDescription bodyD,Ogre::Vector3 pos,Ogre::Quaternion quat)
{
	NxOgre::Matrix44 pose;
	pose.set(PhysicsSystem::GetSingleton()->convertOgreQuat(quat));
	pose.set(PhysicsSystem::GetSingleton()->convertOgreVec3(pos));

	Critter::Body* body = m_pPhyXRenderSys->createBody(shapeD,pose,ent,bodyD);

	return createActor(ent,body);
}

bool ActorManager::removeActor(Actor* toDel)
{
	ActorsList::iterator iter=std::find(m_Actors.begin(),m_Actors.end(),toDel);
	if(iter!=m_Actors.end())
	{
		if(*iter)
		{
			m_pPhyXRenderSys->destroyBody((*iter)->getBody());
			delete *iter;
		}
		m_Actors.erase(iter);
		return true;
	}

	return false;
}

void ActorManager::proccessDeleteQueue()
{
	while(!m_toDelete.empty())
	{
		Actor* toDel = m_toDelete.front();
		ActorsList::iterator iter=std::find(m_Actors.begin(),m_Actors.end(),toDel);
		//ActorsList::iterator iter;
		//iter = m_toDelete.front();
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//This line causes a crash sometimes.
		//I think when there is other projectiles is the queue or in vector
		// UPDATE: it seems to be fixed but not sure
		// I think the problem was that when we had a queue of iterators, once one iter was erased
		// it may change the order of the items so the other iters would be invalid
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		Critter::Body* bod = toDel->getBody();
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		m_pPhyXRenderSys->destroyBody(bod);
		//HACK: not sure if Critter destroys the Ogre::Entity and Ogre::SceneNode with body
		delete *iter;
		m_Actors.erase(iter);
		m_toDelete.pop();
	}
}

void ActorManager::update(float fElapsedTime)
{
	proccessDeleteQueue();

	//HACK:
	//very simple update procedure i.e update every Actor
	//we should do something like:
	//based on the distance from the player timestep the Actor updates so Actor far away from the player don't need to be updated as often

	m_bActorsDataLocked=true;		//setting to true while we iterate
	ActorsList::iterator iter;
	for(iter=m_Actors.begin();iter!=m_Actors.end();iter++)
	{
		(*iter)->update(fElapsedTime);
	}
	m_bActorsDataLocked=false;
	//append the m_tempNewActors to m_Actors
	m_Actors.insert(m_Actors.end(),m_tempNewActors.begin(),m_tempNewActors.end());
	m_tempNewActors.clear();
}

bool ActorManager::queueToDelete(Actor* toDel)
{
	return false;
}

bool ActorManager::queueToDelete(Critter::Body* toDel)
{
	ActorsList::iterator del = std::find(m_Actors.begin(),m_Actors.end(), getActorFromBody(toDel) );

	if(del != m_Actors.end())
	{
		(*del)->setToBeDeleted(true);
		m_toDelete.push(*del);
		return true;
	}
	return false;
}

Actor* ActorManager::getActorFromBody(Critter::Body* bod)
{
	ActorsList::iterator iter=m_Actors.begin();
	bool bFound=false;
	for(;iter!=m_Actors.end() && !bFound;iter++)
	{
		if((*iter)->getBody()==bod)
		{
			bFound=true;
			break;
		}
	}
	if(bFound)
		return *iter;
	else
		return 0;
}

void ActorManager::setPlayerActor(Actor* player)
{
	BOOST_ASSERT(player!=0);
	m_pPlayerActor = player;
}