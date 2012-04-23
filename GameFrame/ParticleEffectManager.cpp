#include "ParticleEffectManager.h"
#include "boost\lexical_cast.hpp"

ParticleEffectManager* Simpleton<ParticleEffectManager>::s_singleton =0;

ParticleEffectManager::ParticleEffectManager(void):
	m_pSceneManager(0),
	ID(0)
{
	m_managedParticles.clear();
	m_unManagedParticles.clear();
	m_Particles.clear();
}

ParticleEffectManager::~ParticleEffectManager(void)
{
	if(!m_Particles.empty())
	{
		clearAll();
	}
	m_managedParticles.clear();
	m_unManagedParticles.clear();
	m_followParticles.clear();
	m_ObjectNodeMap.clear();
	if(m_pSceneManager)
	{
		m_pSceneManager=0;
	}

	ID=0;
}

Ogre::ParticleSystem* ParticleEffectManager::spawnEffect(Ogre::String particleName,Ogre::Vector3 pos,Ogre::Quaternion ori,bool managed)
{
	Ogre::SceneNode* node = m_pSceneManager->getRootSceneNode()->createChildSceneNode(pos,ori);
	return spawnEffect(particleName,node,managed);
}

Ogre::ParticleSystem* ParticleEffectManager::spawnEffect(Ogre::String particleName,Ogre::SceneNode* node,bool managed)
{
	Ogre::ParticleSystem* ps;
	ps = m_pSceneManager->createParticleSystem(generateID(),particleName);
	node->attachObject(ps);

	m_Particles.push_back(ps);
	if(managed)
	{
		ps->_update(0.1f);					//update it so that there's atleast one particle so the manager does'nt delete thinking its dead
		m_managedParticles.push_back(ps);
	}
	else
		m_unManagedParticles.push_back(ps);
	return ps;
}

Ogre::ParticleSystem* ParticleEffectManager::spawnFollowEffect(Ogre::String particleName,Ogre::MovableObject* object)
{
	Ogre::ParticleSystem* ps;
	Ogre::SceneNode* node = object->getParentSceneNode();

	ps = m_pSceneManager->createParticleSystem(generateID(),particleName);
	node->attachObject(ps);
	//add to general particle list
	m_Particles.push_back(ps);

	//check if object exist in our map
	ParticleNodeMap::iterator iter;
	iter = m_followParticles.find(object);
	//if not found
	if(iter==m_followParticles.end())
	{
		ParticleList list;
		list.push_back(ps);
		m_followParticles[object] = list;
	}else	//if found
	{
		iter->second.push_back(ps);
	}
	object->setListener(this); //set listener to this class

	//add to object sceneNode map
	ObjectNodeMap::iterator onIter;
	onIter = m_ObjectNodeMap.find(object);
	if(onIter==m_ObjectNodeMap.end())
	{
		m_ObjectNodeMap[object] = object->getParentSceneNode();
	}

	return ps;
}

void ParticleEffectManager::removeDeadParticleSystems()
{
	if(!m_managedParticles.empty() && !m_Particles.empty())
	{
		ParticleList::iterator iter;
		for(iter=m_managedParticles.begin();iter!=m_managedParticles.end();)
		{
			int dP=(*iter)->getNumParticles();
			if((*iter)->getNumParticles() == 0)
			{
				ParticleList::iterator iterP = std::find(m_Particles.begin(),m_Particles.end(),*iter);
				if(iterP != m_Particles.end())
				{
					Ogre::SceneNode* nodeToDelete = (*iterP)->getParentSceneNode();
					m_pSceneManager->destroyParticleSystem(*iterP);
					if(nodeToDelete->numAttachedObjects()==0)	//if no objects attached then delete node
						m_pSceneManager->destroySceneNode(nodeToDelete);
				}
				ParticleList::iterator toDel=iter;

				iter++;
				m_Particles.erase(iterP);
				m_managedParticles.erase(toDel);
			}else
			{
				iter++;
			}
		}
	}
}

Ogre::String ParticleEffectManager::generateID()
{
	Ogre::String str = PARTICLE_GEN_ID+boost::lexical_cast<Ogre::String,unsigned int>(ID);
	ID++;
	return str;
}

void ParticleEffectManager::clearAll()
{
	if(m_pSceneManager)
		m_pSceneManager->destroyAllParticleSystems();
	m_Particles.clear();
	m_managedParticles.clear();
	ID=0;
	m_pSceneManager=0;
}

int ParticleEffectManager::getNumManagedParticles()const
{
	return m_managedParticles.size();
}

int ParticleEffectManager::getNumUnManagedParticles()const
{
	return m_unManagedParticles.size();
}

int ParticleEffectManager::getTotalParticles()const
{
	return m_Particles.size();
}

void ParticleEffectManager::objectDetached(Ogre::MovableObject* object)
{
	if(!m_pSceneManager)	//if sceneManager is null then no need the clean up is triggering this
		return;
	//check if object exist in our map
	ParticleNodeMap::iterator iter;
	iter = m_followParticles.find(object);

	if(iter != m_followParticles.end())
	{
		Ogre::SceneNode* parent = m_ObjectNodeMap.find(object)->second;
		Ogre::SceneNode* newNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(parent->getPosition()
			,parent->getOrientation());
		ParticleList list =iter->second;
		//loop though the list and stop particle emission and add each system to the managed list and attach each to newNode
		ParticleList::iterator listIter;
		for(listIter=list.begin();listIter!=list.end();listIter++)
		{
			(*listIter)->detachFromParent();
			(*listIter)->setEmitting(false);
			newNode->attachObject(*listIter);
			m_managedParticles.push_back(*listIter);
		}
		list.clear();
		m_ObjectNodeMap.erase(m_ObjectNodeMap.find(object));
		m_followParticles.erase(iter);
	}
}