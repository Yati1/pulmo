#ifndef SOUND_NODE_MANAGER_H
#define SOUND_NODE_MANAGER_H

#include <Ogre.h>
#include "SoundNode.h"
#include <map>
#include "Simpleton.h"

#define GEN_ID_PREFIX "GEN_"

class SoundNodeManager: public Simpleton<SoundNodeManager>
{
public:
	SoundNodeManager(void);
	~SoundNodeManager(void);
	/**
		@param name The name of the node. Can be used to later get the SoundNode
		@param fileName The name of the sound file to be played

		@note	The file name should be a path relative to the path specified in SoundSystem::setFileSystem

		@note	The file system we will use is media/sounds
	*/
	SoundNode* createSoundNode(Ogre::String name,Ogre::String fileName,bool looping = false,float size = 1.0f);

	SoundNode* createSoundNode(Ogre::String fileName,bool looping = false,float size = 1.0f);

	SoundNode* getSoundNode(Ogre::String name)const;
	SoundNode* getSoundNode(SoundNode* pNode)const;

	bool removeSoundNode(SoundNode* pNode);
	bool removeSoundNode(Ogre::String name);

	void playAllSounds();

	void emptyManager();
	/**
		Internal function called by SoundSystem to update the 3d position of the SoundNodes
		with FMOD
	*/
	void _update(float fElapsedTime);

private:

	Ogre::String generateID();

	typedef std::map<Ogre::String,SoundNode*>SNMap;
	SNMap m_mapSNodes;

	unsigned int m_nIDCounter;

	friend class Simpleton< SoundNodeManager >;
};
#endif