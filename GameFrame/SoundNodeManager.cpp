#include "SoundNodeManager.h"
#include "boost\lexical_cast.hpp"

SoundNodeManager* Simpleton<SoundNodeManager>::s_singleton=0;

SoundNodeManager::SoundNodeManager(void):
	m_nIDCounter(0)
{
	m_mapSNodes.clear();
}

SoundNodeManager::~SoundNodeManager(void)
{
	emptyManager();
}

SoundNode* SoundNodeManager::createSoundNode(Ogre::String name,Ogre::String fileName,bool looping,float size)
{
	//if the name doesn't exist
	if(!getSoundNode(name))
	{
		m_mapSNodes[name]=new SoundNode(name,fileName,size,looping);
		return m_mapSNodes[name];
	}else
		return 0;
}

SoundNode* SoundNodeManager::createSoundNode(Ogre::String fileName,bool looping,float size)
{
	Ogre::String name=generateID();

	//if the name doesn't exist
	if(!getSoundNode(name))
	{
		m_mapSNodes[name]=new SoundNode(name,fileName,size,looping);
		return m_mapSNodes[name];
	}else
		return 0;
}

SoundNode* SoundNodeManager::getSoundNode(Ogre::String name)const
{
	SNMap::const_iterator iter;
	iter=m_mapSNodes.find(name);
	if(iter!=m_mapSNodes.end())
		return iter->second;
	return 0;
}
SoundNode* SoundNodeManager::getSoundNode(SoundNode* pNode)const
{
	;//HACK: Not done yet
	return 0;
}

bool SoundNodeManager::removeSoundNode(SoundNode* pNode)
{
	;//HACK: Not done yet
	return false;
}

bool SoundNodeManager::removeSoundNode(Ogre::String name)
{
	SNMap::iterator iter;
	iter=m_mapSNodes.find(name);
	if(iter!=m_mapSNodes.end())
	{
		m_mapSNodes.erase(iter);
		return true;
	}else
		return false;
}

void SoundNodeManager::emptyManager()
{
	SNMap::iterator iter;
	for(iter=m_mapSNodes.begin();iter!=m_mapSNodes.end();iter++)
	{
		SoundNode* toDelete=0;
		if(iter->second)
		{
			toDelete=iter->second;
			iter->second=0;
		}
		//delete toDelete;
	}
	m_mapSNodes.clear();

	m_nIDCounter=0;
}
/**
	Internal function called by SoundSystem to update the 3d position of the SoundNodes
	with FMOD
*/
void SoundNodeManager::_update(float fElapsedTime)
{
	SNMap::const_iterator iter;
	for(iter=m_mapSNodes.begin();iter!=m_mapSNodes.end();iter++)
	{
		if(iter->second->getChannel())
		{
			bool playing = false;
			iter->second->getChannel()->isPlaying(&playing);
			if(playing)
			{
				iter->second->_update(fElapsedTime);
				FMOD_VECTOR fpos,fvel;
				Ogre::Vector3 pos,vel;

				pos=iter->second->getParentSceneNode()->getPosition();
				vel=iter->second->_getVel();

				fpos.x=iter->second->_getFMODPos()->x;
				fpos.y=iter->second->_getFMODPos()->y;
				fpos.z=iter->second->_getFMODPos()->z;

				fvel.x=0;
				fvel.y=0;
				fvel.z=0;

				iter->second->getChannel()->set3DAttributes(&fpos,&fvel);
			}
		}
	}
}

Ogre::String SoundNodeManager::generateID()
{
	Ogre::String str = "GEN_"+boost::lexical_cast<Ogre::String,unsigned int>(m_nIDCounter);
	m_nIDCounter++;
	return str;
}

void SoundNodeManager::playAllSounds()
{
	SNMap::const_iterator iter;
	for(iter=m_mapSNodes.begin();iter!=m_mapSNodes.end();iter++)
	{
		iter->second->play(iter->second->isLooping());
	}
}