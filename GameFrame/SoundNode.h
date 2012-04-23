#ifndef SOUND_NODE_H
#define SOUND_NODE_H

#include <Ogre.h>

#include "SoundSystem.h"

/*******************************

	A SoundNode can be attached to a Ogre::SceneNode.
	The sound needs to be loaded before it is loaded
*******************************/
class SoundNode:public Ogre::MovableObject
{
public:

	class SoundNodeListener:public Ogre::MovableObject::Listener
	{
		//SoundNodeListener(void){}
		void objectMoved(SoundNode* object);
	};

	SoundNode();
	SoundNode(const Ogre::String& name,float size = 1.0f,bool looping = false);
	SoundNode(const Ogre::String& name,const Ogre::String& soundFile,float size = 1.0f,bool looping = false);

	~SoundNode();

	void setSound(const char* fileName);
	FMOD::Channel* getChannel(){return m_pChannel;}

	Ogre::String getSoundFileName(){return m_strSoundFile;}
	FMOD::Sound* getSound(){return m_pSound;}

	void play(bool bLoop=false,bool bPaused=false);
	void stop();

	Ogre::Vector3 _getLastPos(){return m_v3LastPos;}
	Ogre::Vector3 _getVel(){return m_v3Vel;}

	FMOD_VECTOR* _getFMODPos(){return &m_FMOD_pos;}
	FMOD_VECTOR* _getFMODVel(){return &m_FMOD_vel;}

	bool isLooping()const{return m_bLooping;}
	void setLooping(bool looping){m_bLooping = looping;}

	void _update(float elapsedTime);
	//implement Ogre::MovableObject interface

	void _updateRenderQueue(	Ogre::RenderQueue * 	queue ){}

	const Ogre::AxisAlignedBox& getBoundingBox(void)const;

	Ogre::Real getBoundingRadius(void)const;

	const Ogre::String& getMovableType(void)const{return "SoundNode";}

	void visitRenderables(Ogre::Renderable::Visitor* visitor,bool debugRenderables = false){}

private:
	Ogre::String m_strSoundFile;
	FMOD::Sound* m_pSound;			//TODO: add a vector/map of sounds? So 1 SoundNode : many sounds?
	FMOD::Channel* m_pChannel;

	Ogre::Vector3 m_v3LastPos;		//HACK: until physics is put in we will keep the last pos and compute the vel
	Ogre::Vector3 m_v3Vel;

	FMOD_VECTOR m_FMOD_pos;
	FMOD_VECTOR m_FMOD_vel;

	bool m_bLooping;
	float m_fSize;

	SoundNodeListener m_listener;
};

#endif